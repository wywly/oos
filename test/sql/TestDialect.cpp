//
// Created by sascha on 2/26/16.
//

#include "TestDialect.hpp"

#include "sql/dialect_token.hpp"
#include "sql/column.hpp"

#include "sql/types.hpp"

#include <algorithm>

using namespace oos;

const char* TestDialect::type_string(data_type_t type) const
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
      return "DOUBLE";
    case type_double:
      return "DOUBLE";
    case type_char_pointer:
      return "VARCHAR";
    case type_varchar:
      return "VARCHAR";
    case type_text:
      return "TEXT";
    case type_date:
      return "REAL";
    case type_time:
      return "TEXT";
    default:
    {
      std::stringstream msg;
      msg << "test dialect: unknown data type [" << type << "]";
      throw std::logic_error(msg.str());
      //throw std::logic_error("mysql sql: unknown type");
    }
  }
}

std::string TestDialect::compile(const oos::detail::top &top)
{
  std::stringstream res;
  res << token(top.type) << " " << top.limit_ << " ";
  return res.str();
}

std::string TestDialect::compile(const oos::detail::remove &remove1)
{
  return token(remove1.type) + " " + remove1.table + " ";
}

std::string TestDialect::compile(const oos::detail::values &values)
{
  std::string result(token(values.type) + " (");

  if (values.values_.size() > 1) {
    std::for_each(values.values_.begin(), values.values_.end() - 1, [&](const std::shared_ptr<detail::basic_value> &val)
    {
      result += val->compile(*this);
      result += ", ";
    });
  }
  if (!values.values_.empty()) {
    result += values.values_.back()->compile(*this);
  }
  result += ") ";
  return result;
}

std::string TestDialect::compile(const oos::detail::basic_value &value)
{
  return value.str();
}

std::string TestDialect::compile(const oos::detail::insert &insert)
{
  return token(insert.type) + " " + insert.table + " ";
}

std::string TestDialect::compile(const oos::detail::group_by &by)
{
  return token(by.type) + " " + by.column + " ";
}

std::string TestDialect::compile(const oos::detail::desc &desc)
{
  return token(desc.type) + " ";
}

std::string TestDialect::compile(const oos::detail::asc &asc)
{
  return token(asc.type) + " ";
}

std::string TestDialect::compile(const oos::detail::order_by &by)
{
  return token(by.type) + " " + by.column + " ";
}

std::string TestDialect::compile(const oos::detail::basic_condition &cond)
{
  return cond.compile(*this);
}

std::string TestDialect::compile(const oos::detail::where &where)
{
  return token(where.type) + " " + where.cond->compile(*this) + " ";
}

std::string TestDialect::compile(const oos::detail::from &from)
{
  return token(from.type) + " " + from.table + " ";
}

std::string TestDialect::compile(const oos::detail::varchar_column &column)
{
  std::stringstream str;
  str << column.name << " " << type_string(column.type) << "(" << column.size << ")";
  return str.str();
}

std::string TestDialect::compile(const oos::detail::identifier_column &column)
{
  return column.name + " " + type_string(column.type) + " NOT NULL PRIMARY KEY";
}

std::string TestDialect::compile(const oos::column &column)
{
  return column.name;
}

std::string TestDialect::compile(const oos::detail::typed_column &column)
{
  return column.name + " " + type_string(column.type);
}

std::string TestDialect::compile(const oos::detail::columns &columns)
{
  std::string result;
  if (columns.with_brackets()) {
    result += "(";
  }

  if (columns.columns_.size() > 1) {
    std::for_each(columns.columns_.begin(), columns.columns_.end() - 1, [&](const std::shared_ptr<column> &col)
    {
      result += col->compile(*this);
      result += ", ";
    });
  }
  if (!columns.columns_.empty()) {
    result += columns.columns_.back()->compile(*this);
  }

  if (columns.with_brackets()) {
    result += ")";
  }

  result += " ";
  return result;
}

std::string TestDialect::compile(const oos::detail::set &set)
{
  return token(set.type) + " ";
}

std::string TestDialect::compile(const oos::detail::update &update)
{
  return token(update.type) + " " + update.table + " ";
}

std::string TestDialect::compile(const oos::detail::distinct &distinct)
{
  return token(distinct.type) + " ";
}

std::string TestDialect::compile(const oos::detail::select &select1)
{
  return token(select1.type) + " ";
}

std::string TestDialect::compile(const oos::detail::drop &drop)
{
  return token(drop.type) + " " + drop.table + " ";
}

std::string TestDialect::compile(const oos::detail::create &create)
{
  return token(create.type) + " " + create.table + " ";
}