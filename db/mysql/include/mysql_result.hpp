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

#ifndef MYSQL_RESULT_HPP
#define MYSQL_RESULT_HPP

#include "database/result_impl.hpp"

#ifdef WIN32
#include <winsock2.h>
#include <mysql.h>
#else
#include <mysql/mysql.h>
#endif

#include <vector>

namespace oos {

class row;
class serializable;

namespace mysql {

class mysql_result : public detail::result_impl
{
private:
  mysql_result(const mysql_result&) = delete;
  mysql_result& operator=(const mysql_result&) = delete;

public:
  typedef detail::result_impl::size_type size_type;

public:
  mysql_result(std::shared_ptr<object_base_producer> producer, MYSQL *c);
  virtual ~mysql_result();
  
  const char* column(size_type c) const;
  bool fetch();
  
  bool fetch(serializable *o);

  size_type affected_rows() const;
  size_type result_rows() const;
  size_type fields() const;

  virtual int transform_index(int index) const;

protected:
  virtual void read(const char *id, char &x);
  virtual void read(const char *id, short &x);
  virtual void read(const char *id, int &x);
  virtual void read(const char *id, long &x);
  virtual void read(const char *id, unsigned char &x);
  virtual void read(const char *id, unsigned short &x);
  virtual void read(const char *id, unsigned int &x);
  virtual void read(const char *id, unsigned long &x);
  virtual void read(const char *id, bool &x);
  virtual void read(const char *id, float &x);
  virtual void read(const char *id, double &x);
  virtual void read(const char *id, char *x, size_t s);
  virtual void read(const char *id, varchar_base &x);
  virtual void read(const char *id, std::string &x);
  virtual void read(const char *id, oos::date &x);
  virtual void read(const char *id, oos::time &x);
  virtual void read(const char *id, object_base_ptr &x);
  virtual void read(const char *id, object_container &x);
  virtual void read(const char *id, basic_identifier &x);

private:
  struct result_deleter
  {
    void operator()(MYSQL_RES *res) const {
      if (res) {
        mysql_free_result(res);
      }
    }
  };
  size_type affected_rows_;
  size_type rows_;
  size_type fields_;
  MYSQL_ROW row_;
  MYSQL_RES *res_;
};

}

}

#endif /* MYSQL_RESULT_HPP */
