//
// Created by sascha on 20.03.16.
//

#include "orm/persistence.hpp"

namespace oos {


persistence::persistence(const std::string &dns)
  : connection_(dns)
{
  connection_.open();
}

persistence::~persistence()
{
  connection_.close();
}

void persistence::create()
{
  for (t_table_map::value_type &val : tables_) {
    if (connection_.exists(val.second.name())) {
      continue;
    }
    val.second.create(connection_);
  }
}

void persistence::drop()
{
  for (t_table_map::value_type &val : tables_) {
    if (!connection_.exists(val.second.name())) {
      continue;
    }
    val.second.drop(connection_);
  }
}

object_store &persistence::store()
{
  return store_;
}

const object_store &persistence::store() const
{
  return store_;
}


}