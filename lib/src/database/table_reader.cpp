#include "database/result.hpp"
#include "database/database_exception.hpp"
#include "database/table_reader.hpp"
#include "database/table.hpp"

namespace oos {

table_reader::table_reader(table &t, object_store &ostore)
  : generic_object_reader(this)
  , ostore_(ostore)
  , object_preparator_(t.node_)
  , table_(t)
{}


void table_reader::read(result *res)
{

  serializable *obj = nullptr;

  while ((obj = res->fetch(&table_.node_))) {

    if (table_.node_.has_primary_key()) {
      
    }

    new_proxy_ = new object_proxy(obj, nullptr);

    obj->deserialize(*this);

    ostore_.insert_proxy(new_proxy_);
  }


  // check result
  // create serializable
//  std::unique_ptr<serializable> obj(table_.node_.producer->create());
//
//  // prepare serializable for read (set object_proxy into serializable ptr)
//  object_preparator_.prepare(obj.get());
//
////  std::for_each(res->begin(), res->end(), [](serializable *obj) {})
//  while (res->fetch(obj.get())) {
//
//    new_proxy_ = new object_proxy(obj.get(), nullptr);
//
//    obj->deserialize(*this);
//
//    obj.release();
//
//    ostore_.insert_proxy(new_proxy_);
//
//    obj.reset(table_.node_.producer->create());
//  }
}

void table_reader::read_value(const char */*id*/, object_base_ptr &x)
{
  std::shared_ptr<primary_key_base> pk = x.primary_key();
  if (!pk) {
    return;
  }

  // get node of object type
  prototype_iterator node = ostore_.find_prototype(x.type());

  /**
   * find proxy in node map
   * if proxy can be found object was
   * already read - replace proxy
   */
  object_proxy *proxy = node->find_proxy(pk);
  if (proxy) {
    x.reset(proxy, x.is_reference());
  } else {
    x.proxy_->obj_ = node->producer->create();
    proxy = ostore_.register_proxy(x.proxy_);
  }


  /*
   * add the child serializable to the serializable proxy
   * of the parent container
   */
  database::table_map_t::iterator j = table_.db_.table_map_.find(node->type);
  prototype_node::field_prototype_map_t::const_iterator i = table_.node_.relations.find(node->type);
  if (i != table_.node_.relations.end()) {
    j->second->relation_data[i->second.second][proxy->id()].push_back(new_proxy_);
  }
}

void table_reader::read_value(const char *id, object_container &x)
{
  /*
   * find prototype node and check if there
   * are proxies to insert for this container
   */
  prototype_iterator p = ostore_.find_prototype(x.classname());
  if (p == ostore_.end()) {
    throw database_exception("common", "couldn't find prototype node");
  }
  if (!table_.db_.is_loaded(p->type)) {
    return;
  }
  table::t_to_many_data::iterator i = table_.relation_data.find(id);
  if (i == table_.relation_data.end()) {
    return;
  }
  /*
   * find object for container in new object
   */
  table::object_map_t::iterator j = i->second.find(new_proxy_->id());
  if (j == i->second.end()) {
    // no objects found
    return;
  }
  // move object into container
  while (!j->second.empty()) {
    x.append_proxy(j->second.front());
    j->second.pop_front();
  }
}

}
