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

#include "tools/varchar.hpp"
#include "tools/date.hpp"
#include "tools/time.hpp"

#include "object/identifier.hpp"
#include "object/serializable.hpp"
#include "object/object_ptr.hpp"

#include "sqlite_result.hpp"

#include <cstring>
#include <algorithm>

namespace oos {

namespace sqlite {

sqlite_result::sqlite_result(std::shared_ptr<object_base_producer> producer)
  : detail::result_impl(producer)
{}

sqlite_result::~sqlite_result()
{
  std::for_each(result_.begin(), result_.end(), [](t_result::value_type& row) {
    std::for_each(row.begin(), row.end(), [](char *val) {
      delete [] val;
    });
  });
}

const char* sqlite_result::column(sqlite_result::size_type c) const
{
  t_row::value_type val = result_.at(pos_).at(c);
  return val;
}

bool sqlite_result::fetch()
{
  return ++pos_ < result_.size();
}

bool sqlite_result::fetch(serializable *obj)
{
  if (pos_ + 1 > result_.size()) {
    return false;
  }

  column_ = 0;
  obj->deserialize(*this);
  ++pos_;

  return true;
}

sqlite_result::size_type sqlite_result::affected_rows() const
{
  return 0;
}

sqlite_result::size_type sqlite_result::result_rows() const
{
  return result_.size();
}

sqlite_result::size_type sqlite_result::fields() const
{
  return 0;
}

int sqlite_result::transform_index(int index) const
{
  return index;
}

void sqlite_result::push_back(char **row_values, int column_count)
{
  t_row row;
  for(int i = 0; i < column_count; ++i) {
    // copy and store column data;
    if (row_values[i] == nullptr) {
      auto val = new char[1];
      val[0] = '\0';
      row.push_back(val);
    } else {
      size_t size = strlen(row_values[i]);
      auto val = new char[size + 1];
      std::memcpy(val, row_values[i], size);
      val[size] = '\0';
      row.push_back(val);
    }
  }
  result_.push_back(row);
}

void sqlite_result::read(const char */*id*/, char &x)
{
  t_row::value_type &val = result_[pos_][column_++];

  if (strlen(val) > 1) {
    x = val[0];
  }
}

void sqlite_result::read(const char */*id*/, short &x)
{
  t_row::value_type &val = result_[pos_][column_++];
  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = (short)strtol(val, &end, 10);
  // Todo: check error
}

void sqlite_result::read(const char */*id*/, int &x)
{
  t_row::value_type &val = result_[pos_][column_++];
  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = (int)strtol(val, &end, 10);
  // Todo: check error
}

void sqlite_result::read(const char */*id*/, long &x)
{
  t_row::value_type &val = result_[pos_][column_++];
  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = strtol(val, &end, 10);
  // Todo: check error
}

void sqlite_result::read(const char */*id*/, unsigned char &x)
{
  t_row::value_type &val = result_[pos_][column_++];
  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = (unsigned char)strtoul(val, &end, 10);
  // Todo: check error
}

void sqlite_result::read(const char */*id*/, unsigned short &x)
{
  t_row::value_type &val = result_[pos_][column_++];
  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = (unsigned short)strtoul(val, &end, 10);
  // Todo: check error
}

void sqlite_result::read(const char */*id*/, unsigned int &x)
{
  t_row::value_type &val = result_[pos_][column_++];
  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = (unsigned int)strtoul(val, &end, 10);
  // Todo: check error
}

void sqlite_result::read(const char */*id*/, unsigned long &x)
{
  char *val = result_[pos_][column_++];
  if (strlen(val) == 0) {
    return;
  }
  char *end = nullptr;
  x = strtoul(val, &end, 10);
  // Todo: check error
}

void sqlite_result::read(const char */*id*/, bool &x)
{
  t_row::value_type &val = result_[pos_][column_++];
  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = strtoul(val, &end, 10) > 0;
  // Todo: check error
}

void sqlite_result::read(const char */*id*/, float &x)
{
  t_row::value_type &val = result_[pos_][column_++];
  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = (float)strtod(val, &end);
  // Todo: check error
}

void sqlite_result::read(const char */*id*/, double &x)
{
  t_row::value_type &val = result_[pos_][column_++];
  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = strtod(val, &end);
  // Todo: check error
}

void sqlite_result::read(const char */*id*/, char *x, size_t s)
{
  t_row::value_type &val = result_[pos_][column_++];
  size_t len = strlen(val);
  if (len > (size_t)s) {
    strncpy(x, val, s);
    x[s-1] = '\n';
  } else {
    strcpy(x, val);
  }
}

void sqlite_result::read(const char */*id*/, varchar_base &x)
{
  t_row::value_type val = result_[pos_][column_++];
  x.assign(val);
}

void sqlite_result::read(const char */*id*/, std::string &x)
{
  t_row::value_type val = result_[pos_][column_++];
  x.assign(val);
}

void sqlite_result::read(const char *id, oos::date &x)
{
  double val = 0;
  read(id, val);
  x.set(static_cast<int>(val));
}

void sqlite_result::read(const char *id, oos::time &x)
{
std::string val;
read(id, val);
x = oos::time::parse(val, "%F %T.%f");
}

void sqlite_result::read(const char *id, object_base_ptr &x)
{
  read_foreign_object(id, x);
}

void sqlite_result::read(const char */*id*/, object_container &/*x*/)
{
}

void sqlite_result::read(const char *id, basic_identifier &x)
{
  x.deserialize(id, *this);
}

}

}
