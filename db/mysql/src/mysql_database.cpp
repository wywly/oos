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

#include "mysql_database.hpp"
#include "mysql_statement.hpp"
#include "mysql_result.hpp"
#include "mysql_types.hpp"
#include "mysql_exception.hpp"

#include "database/session.hpp"
#include "database/database_sequencer.hpp"
#include "database/row.hpp"

#include <sstream>

using namespace std::placeholders;

namespace oos {
  
namespace mysql {
  
mysql_database::mysql_database(session *db)
  : database(db, new database_sequencer(*this))
  , is_open_(false)
{
}

mysql_database::~mysql_database()
{
  close();
}


void mysql_database::on_open(const std::string &connection)
{
  // parse user[:passwd]@host/db
  
  std::string con = connection;
  std::string::size_type pos = con.find('@');
  std::string user, passwd;
  bool has_pwd = true;
  if (pos == std::string::npos) {
	  throw_error("mysql:open", "invalid dsn: " + connection);
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
      has_pwd = false;
    }
  }
  con = con.substr(pos + 1);
  pos = con.find('/');
  std::string host = con.substr(0, pos);
  std::string db = con.substr(pos + 1);

  if (mysql_init(&mysql_) == 0) {
    throw_error("mysql", "initialization failed");
  }
  
  if (mysql_real_connect(&mysql_, host.c_str(), user.c_str(), (has_pwd ? passwd.c_str() : 0), db.c_str(), 0, NULL, 0) == NULL) {
    // close all handles
    mysql_close(&mysql_);
    // throw exception
    throw_error(-1, &mysql_, "mysql");
  }
  is_open_ = true;
}

bool mysql_database::is_open() const
{
  return is_open_;
}

void mysql_database::on_close()
{
  mysql_close(&mysql_);
  // tell mysql to close the library
  mysql_library_end();

  is_open_ = false;
}

MYSQL* mysql_database::operator()()
{
  return &mysql_;
}

detail::result_impl* mysql_database::on_execute(const std::string &sqlstr, std::shared_ptr<object_base_producer> ptr)
{
  if (mysql_query(&mysql_, sqlstr.c_str())) {
    throw mysql_exception(&mysql_, "mysql_query", sqlstr);
  }
  return new mysql_result(ptr, &mysql_);
}

detail::statement_impl* mysql_database::on_prepare(const oos::sql &stmt, std::shared_ptr<object_base_producer> ptr)
{
  return new mysql_statement(*this, stmt, ptr);
}

void mysql_database::on_begin()
{
  auto res = execute<serializable>("START TRANSACTION;", (std::shared_ptr<object_base_producer>()));
  // TODO: check result
}

void mysql_database::on_commit()
{
  auto res = execute<serializable>("COMMIT;", (std::shared_ptr<object_base_producer>()));
  // TODO: check result
}

void mysql_database::on_rollback()
{
  auto res = execute<serializable>("ROLLBACK;", (std::shared_ptr<object_base_producer>()));
  // TODO: check result
}

const char* mysql_database::type_string(data_type_t type) const
{
  switch(type) {
    case type_char:
      return "INTEGER";
    case type_short:
      return "INTEGER";
    case type_int:
      return "INTEGER";
    case type_long:
      return "INTEGER";
    case type_unsigned_char:
      return "INTEGER";
    case type_unsigned_short:
      return "INTEGER";
    case type_unsigned_int:
      return "INTEGER";
    case type_unsigned_long:
      return "INTEGER";
    case type_bool:
      return "INTEGER";
    case type_float:
      return "FLOAT";
    case type_double:
      return "DOUBLE";
    case type_date:
      return "DATE";
    case type_time:
#if MYSQL_VERSION_ID < 50604
      // before mysql version 5.6.4 datetime
      // doesn't support fractional seconds
      // so we use a datetime string here
      return "VARCHAR(256)";
#else
      return "DATETIME(3)";
#endif
    case type_char_pointer:
      return "VARCHAR";
    case type_varchar:
      return "VARCHAR";
    case type_text:
      return "TEXT";
    default:
      {
        std::stringstream msg;
        msg << "mysql database: unknown type xxx [" << type << "]";
        throw std::logic_error(msg.str());
        //throw std::logic_error("mysql database: unknown type");
      }
    }
}

unsigned long mysql_database::last_inserted_id()
{
  if (mysql_field_count(&mysql_) == 0 &&
      mysql_insert_id(&mysql_) != 0)
  {
    return mysql_insert_id(&mysql_);
  }
  return 0;
}

}

}

extern "C"
{
  OOS_MYSQL_API oos::database* create_database(oos::session *ses)
  {
    return new oos::mysql::mysql_database(ses);
  }

  OOS_MYSQL_API void destroy_database(oos::database *db)
  {
    delete db;
  }
}
