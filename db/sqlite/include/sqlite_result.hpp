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

#ifndef SQLITE_RESULT_HPP
#define SQLITE_RESULT_HPP

#include "database/result_impl.hpp"

#include <vector>

namespace oos {

class row;
class serializable;

namespace sqlite {

class sqlite_result : public detail::result_impl
{
private:
  sqlite_result(const sqlite_result&) = delete;
  sqlite_result& operator=(const sqlite_result&) = delete;

public:
  typedef detail::result_impl::size_type size_type;

public:
  sqlite_result(std::shared_ptr<object_base_producer> producer);
  virtual ~sqlite_result();
  
  const char* column(size_type c) const;
  virtual bool fetch();
  virtual bool fetch(serializable *);
  size_type affected_rows() const;
  size_type result_rows() const;
  size_type fields() const;

  virtual int transform_index(int index) const;

  void push_back(char **row_values, int column_count);

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
  friend class sqlite_database;

private:

//  typedef std::vector<std::shared_ptr<char> > t_row;
  typedef std::vector<char*> t_row;
  typedef std::vector<t_row> t_result;

  t_result result_;
  t_result::size_type pos_ = 0;
  t_result::size_type column_ = 0;

  size_type fields_;
};

}

}

#endif /* SQLITE_RESULT_HPP */
