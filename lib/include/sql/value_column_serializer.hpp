//
// Created by sascha on 03.04.16.
//

#ifndef OOS_VALUE_COLUMN_SERIALIZER_HPP
#define OOS_VALUE_COLUMN_SERIALIZER_HPP

#include "tools/access.hpp"
#include "tools/serializer.hpp"

#include "sql/column.hpp"

namespace oos {

class sql;

namespace detail {

class value_column_serializer : public serializer {
public:
  value_column_serializer(sql &d);

  template<class T>
  void append_to(const std::shared_ptr<detail::columns> cols, T &x)
  {
    cols_ = cols;
    oos::access::serialize(static_cast<serializer&>(*this), x);
  }

  void serialize(const char *id, char &x);
  void serialize(const char *id, short &x);
  void serialize(const char *id, int &x);
  void serialize(const char *id, long &x);
  void serialize(const char *id, unsigned char &x);
  void serialize(const char *id, unsigned short &x);
  void serialize(const char *id, unsigned int &x);
  void serialize(const char *id, unsigned long &x);
  void serialize(const char *id, float &x);
  void serialize(const char *id, double &x);
  void serialize(const char *id, bool &x);
  void serialize(const char *id, char *x, size_t s);
  void serialize(const char *id, varchar_base &x);
  void serialize(const char *id, std::string &x);
  void serialize(const char *id, date &x);
  void serialize(const char *id, time &x);
  void serialize(const char *id, identifiable_holder &x, cascade_type);
  void serialize(const char *id, basic_identifier &x);

private:
  std::shared_ptr<detail::columns> cols_;
  sql &sql_;
};
}
}
#endif //OOS_VALUE_COLUMN_SERIALIZER_HPP