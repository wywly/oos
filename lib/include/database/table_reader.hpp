#ifndef TABLE_READER_HPP
#define TABLE_READER_HPP

#include "object/serializer.hpp"
#include "object/serializable.hpp"

namespace oos {

class object_store;
class object_proxy;

class database;
template < class T > class result;
class table;

/// @cond OOS_DEV

class table_reader : public generic_deserializer<table_reader>
{
public:
  table_reader(table &t, object_store &ostore);
  virtual ~table_reader() {}

  void load(result<serializable> &res);

  template < class T >
  void read_value(const char *, T &) {}
  void read_value(const char *, char *, size_t) {}
  void read_value(const char *, object_base_ptr &x);
  void read_value(const char *id, object_container &x);

private:
  // temp data while loading
  object_proxy *new_proxy_;
  object_store &ostore_;
  table &table_;
};

/// @endcond

}
#endif /* TABLE_READER_HPP */
