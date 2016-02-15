/*
 * This file is part of OpenObjectStore OOS.
 *
 * OpenObjectStore OOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenObjectStore OOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenObjectStore OOS. If not, see <http://www.gnu.org/licenses/>.
 */

#include "mssql_connection.hpp"
#include "mssql_statement.hpp"
#include "mssql_result.hpp"
#include "mssql_types.hpp"

#include "tools/string.hpp"

using namespace std::placeholders;

namespace oos {
  
namespace mssql {

mssql_connection::mssql_connection()
  : odbc_(0)
  , connection_(0)
  , is_open_(false)
  , retries_(1)
{
}

mssql_connection::~mssql_connection()
{
  close();
}


void mssql_connection::open(const std::string &connection)
{
  // parse user[:passwd]@host/db ([Drivername])
  std::string con = connection;
  std::string::size_type pos = con.find('@');
  std::string user, passwd;
  std::string driver;
//  bool has_pwd = true;
  if (pos == std::string::npos) {
	  throw_error("mssql:open", "invalid dsn: " + connection);
  } else {
    // try to find colon (:)
    std::string credentials = con.substr(0, pos);
    std::string::size_type colpos = credentials.find(':');
    if (colpos != std::string::npos) {
      // found colon, extract user and passwd
      user = credentials.substr(0, colpos);
      passwd = credentials.substr(colpos + 1, pos);
    } else {
      // only user name given
      user = credentials.substr(0, pos);
    }
  }
  // get connection part
  con = con.substr(pos + 1);
  pos = con.find('/');
  std::string host = con.substr(0, pos);
  std::string db = con.substr(pos + 1);

  // get driver
  pos = db.find('(');
  if (pos != std::string::npos) {
    driver = db.substr(pos+1);
    db = db.substr(0, pos);
    db = trim(db);
    pos = driver.find(')');
    driver = driver.substr(0, pos);
  } else {
    driver = "SQL Server";
  }

  SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &odbc_);
  if (ret != SQL_SUCCESS) {
    SQLFreeHandle(SQL_HANDLE_ENV, odbc_);
    throw_error(ret, SQL_HANDLE_ENV, odbc_, "mssql", "couldn't get odbc handle");
  }

  ret = SQLSetEnvAttr(odbc_, SQL_ATTR_ODBC_VERSION,(SQLPOINTER)SQL_OV_ODBC3, 0);
  if (ret != SQL_SUCCESS) {
    SQLFreeHandle(SQL_HANDLE_ENV, odbc_);
    throw_error(ret, SQL_HANDLE_ENV, odbc_, "mssql", "couldn't set odbc driver version");
  }

  ret = SQLAllocHandle(SQL_HANDLE_DBC, odbc_, &connection_);
  if (ret != SQL_SUCCESS) {
    SQLFreeHandle(SQL_HANDLE_ENV, odbc_);
    throw_error(ret, SQL_HANDLE_DBC, connection_, "mssql", "couldn't get connection handle");
  }

  SQLSetConnectAttr(connection_, SQL_LOGIN_TIMEOUT, (SQLPOINTER *)5, 0);

  std::string dns("DRIVER={" + driver + "};SERVER=" + host + ",1433;DATABASE=" + db + ";UID=" + user + ";PWD=" + passwd + ";");

  SQLCHAR retconstring[1024];
  ret = SQLDriverConnect(connection_, 0, (SQLCHAR*)dns.c_str(), SQL_NTS, retconstring, 1024, NULL,SQL_DRIVER_NOPROMPT);

  throw_error(ret, SQL_HANDLE_DBC, connection_, "mssql", "error on connect");

  is_open_ = true;
}

bool mssql_connection::is_open() const
{
  return is_open_;
}

void mssql_connection::close()
{
  if (!is_open_) {
    return;
  }

  SQLRETURN ret = SQLDisconnect(connection_);

  throw_error(ret, SQL_HANDLE_DBC, connection_, "mssql", "error on close");

  ret = SQLFreeHandle(SQL_HANDLE_DBC, connection_);
  throw_error(ret, SQL_HANDLE_DBC, connection_, "mssql", "error on freeing connection");

  connection_ = nullptr;

  ret = SQLFreeHandle(SQL_HANDLE_ENV, odbc_);
  throw_error(ret, SQL_HANDLE_ENV, odbc_, "mssql", "error on freeing odbc");

  odbc_ = nullptr;

  is_open_ = false;
}

detail::result_impl* mssql_connection::execute(const std::string &sqlstr)
{
  if (!connection_) {
    throw_error("mssql", "no odbc connection established");
  }
  // create statement handle
  SQLHANDLE stmt;

  SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, connection_, &stmt);
  throw_error(ret, SQL_HANDLE_DBC, connection_, "mssql", "error on creating sql statement");

  // execute statement
//  int retry = retries_;
  ret = SQLExecDirectA(stmt, (SQLCHAR*)sqlstr.c_str(), SQL_NTS);

  /*
  do {
    ret = SQLExecDirectA(stmt, (SQLCHAR*)sqlstr.c_str(), SQL_NTS);
  } while (retry-- && !(SQL_SUCCEEDED(ret) || SQL_NO_DATA));
  */

  throw_error(ret, SQL_HANDLE_STMT, stmt, sqlstr, "error on query execute");

  return new mssql_result(stmt, true);
}

oos::detail::statement_impl *mssql_connection::prepare(const oos::sql &stmt)
{
  return new mssql_statement(*this, stmt);
}

void mssql_connection::begin()
{
  /*auto res = */execute("BEGIN TRANSACTION;");
  // TODO: check result
}

void mssql_connection::commit()
{
  /*auto res = */execute("COMMIT;");
  // TODO: check result
}

void mssql_connection::rollback()
{
  /*auto res = */execute("ROLLBACK;");
  // TODO: check result
}

const char*mssql_connection::type_string(data_type_t type) const
{
  switch(type) {
    case type_char:
      return "CHAR(1)";
    case type_short:
      return "SMALLINT";
    case type_int:
      return "INT";
    case type_long:
      return "BIGINT";
    case type_unsigned_char:
      return "SMALLINT";
    case type_unsigned_short:
      return "INT";
    case type_unsigned_int:
      return "BIGINT";
    case type_unsigned_long:
      return "NUMERIC(21,0)";
    case type_bool:
      return "BIT";
    case type_float:
      return "FLOAT";
    case type_double:
      return "FLOAT";
    case type_char_pointer:
      return "VARCHAR";
    case type_varchar:
      return "VARCHAR";
    case type_text:
      return "TEXT";
    case type_date:
      return "DATE";
    case type_time:
      return "DATETIME";
    default:
      {
        std::stringstream msg;
        msg << "mssql connection: unknown type xxx [" << type << "]";
        throw std::logic_error(msg.str());
      }
    }
}

SQLHANDLE mssql_connection::handle()
{
  return connection_;
}

unsigned long mssql_connection::last_inserted_id()
{
  /*auto res = */execute("select scope_identity()");
  unsigned long id = 0;
//  res.get(0, id);
  return id;
}

}

}

extern "C"
{
  OOS_MSSQL_API oos::connection_impl* create_database()
  {
    return new oos::mssql::mssql_connection();
  }

  OOS_MSSQL_API void destroy_database(oos::connection_impl *db)
  {
    delete db;
  }
}