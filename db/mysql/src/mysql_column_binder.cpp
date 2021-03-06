#include "mysql_column_binder.hpp"

#include "tools/date.hpp"
#include "tools/time.hpp"
#include "tools/varchar.hpp"

#include "object/object_ptr.hpp"

#include <cstring>

namespace oos {

namespace mysql {

mysql_column_binder::mysql_column_binder()
  : column_index_(0)
  , stmt_(nullptr)
  , bind_(nullptr)
  , info_(nullptr)
{}

mysql_column_binder::~mysql_column_binder()
{}

void mysql_column_binder::bind(serializable *o, mysql_result_info *info, MYSQL_STMT *stmt, MYSQL_BIND *bind)
{
  column_index_ = 0;
  stmt_ = stmt;
  bind_ = bind;
  info_ = info;
  o->deserialize(*this);
  // bind result array to statement
  mysql_stmt_bind_result(stmt, bind_);
  info_ = nullptr;
  bind_ = nullptr;
  stmt_ = nullptr;
}

void mysql_column_binder::read(const char *, char &x)
{
  prepare_bind_column(column_index_++, MYSQL_TYPE_TINY, x);
}

void mysql_column_binder::read(const char *, short &x)
{
  prepare_bind_column(column_index_++, MYSQL_TYPE_SHORT, x);
}

void mysql_column_binder::read(const char *, int &x)
{
  prepare_bind_column(column_index_++, MYSQL_TYPE_LONG, x);
}

void mysql_column_binder::read(const char *, long &x)
{
  prepare_bind_column(column_index_++, MYSQL_TYPE_LONGLONG, x);
}

void mysql_column_binder::read(const char *, unsigned char &x)
{
  prepare_bind_column(column_index_++, MYSQL_TYPE_TINY, x);
}

void mysql_column_binder::read(const char *, unsigned short &x)
{
  prepare_bind_column(column_index_++, MYSQL_TYPE_SHORT, x);
}

void mysql_column_binder::read(const char *, unsigned int &x)
{
  prepare_bind_column(column_index_++, MYSQL_TYPE_LONG, x);
}

void mysql_column_binder::read(const char *, unsigned long &x)
{
  prepare_bind_column(column_index_++, MYSQL_TYPE_LONGLONG, x);
}

void mysql_column_binder::read(const char *, bool &x)
{
  prepare_bind_column(column_index_++, MYSQL_TYPE_TINY, x);
}

void mysql_column_binder::read(const char *, float &x)
{
  prepare_bind_column(column_index_++, MYSQL_TYPE_FLOAT, x);
}

void mysql_column_binder::read(const char *, double &x)
{
  prepare_bind_column(column_index_++, MYSQL_TYPE_DOUBLE, x);
}

void mysql_column_binder::read(const char *, char *x, size_t s)
{
  prepare_bind_column(column_index_++, MYSQL_TYPE_VAR_STRING, x, s);
}

void mysql_column_binder::read(const char *, std::string &x)
{
  prepare_bind_column(column_index_++, MYSQL_TYPE_STRING, x);
}

void mysql_column_binder::read(const char *, oos::date &x)
{
  prepare_bind_column(column_index_++, MYSQL_TYPE_DATE, x);
}

void mysql_column_binder::read(const char *, oos::time &x)
{
#if MYSQL_VERSION_ID < 50604
  prepare_bind_column(column_index_++, MYSQL_TYPE_VAR_STRING, x);
#else
  prepare_bind_column(column_index_++, MYSQL_TYPE_TIMESTAMP, x);
#endif
}

void mysql_column_binder::read(const char *, varchar_base &x)
{
  prepare_bind_column(column_index_++, MYSQL_TYPE_VAR_STRING, x);
}

void mysql_column_binder::read(const char *id, object_base_ptr &x)
{
  std::shared_ptr<basic_identifier> pk(x.create_identifier());

  x.reset(pk);

  pk->deserialize(id, *this);
}

void mysql_column_binder::read(const char *, object_container &)
{}


void mysql_column_binder::read(const char *id, basic_identifier &x)
{
  x.deserialize(id, *this);
}

void mysql_column_binder::prepare_bind_column(int index, enum_field_types type, oos::date &)
{
  if (info_[index].buffer == 0) {
    size_t s = sizeof(MYSQL_TIME);
    info_[index].buffer = new char[s];
    memset(info_[index].buffer, 0, s);
    info_[index].buffer_length = s;
  }
  bind_[index].buffer_type = type;
  bind_[index].buffer = info_[index].buffer;
  bind_[index].buffer_length = info_[index].buffer_length;
  bind_[index].is_null = &info_[index].is_null;
  bind_[index].length = &info_[index].length;
  bind_[index].error = &info_[index].error;
}

void mysql_column_binder::prepare_bind_column(int index, enum_field_types type, oos::time &)
{
  if (info_[index].buffer == 0) {
    size_t s = sizeof(MYSQL_TIME);
    info_[index].buffer = new char[s];
    memset(info_[index].buffer, 0, s);
    info_[index].buffer_length = s;
  }
  bind_[index].buffer_type = type;
  bind_[index].buffer = info_[index].buffer;
  bind_[index].buffer_length = info_[index].buffer_length;
  bind_[index].is_null = &info_[index].is_null;
  bind_[index].length = &info_[index].length;
  bind_[index].error = &info_[index].error;
}

void mysql_column_binder::prepare_bind_column(int index, enum_field_types type, std::string & /*value*/)
{
  bind_[index].buffer_type = type;
  bind_[index].buffer = 0;
  bind_[index].buffer_length = 0;
  bind_[index].is_null = &info_[index].is_null;
  bind_[index].length = &info_[index].length;
  bind_[index].error = &info_[index].error;
}

void mysql_column_binder::prepare_bind_column(int index, enum_field_types type, char *x, size_t s)
{
  bind_[index].buffer_type = type;
  bind_[index].buffer= x;
  bind_[index].buffer_length = (unsigned long) s;
  bind_[index].is_null = &info_[index].is_null;
  bind_[index].length = &info_[index].length;
  bind_[index].error = &info_[index].error;
}

void mysql_column_binder::prepare_bind_column(int index, enum_field_types type, varchar_base &x)
{
  if (info_[index].buffer == 0) {
    info_[index].buffer = new char[x.capacity()];
    memset(info_[index].buffer, 0, x.capacity());
    info_[index].buffer_length = x.capacity();
  }
  bind_[index].buffer_type = type;
  bind_[index].buffer = info_[index].buffer;
  bind_[index].buffer_length = info_[index].buffer_length;
  bind_[index].is_null = &info_[index].is_null;
  bind_[index].length = &info_[index].length;
  bind_[index].error = &info_[index].error;
}

}

}
