#include "ObjectPrototypeTestUnit.hpp"

#include "object/object_store.hpp"

#include "../Item.hpp"

#include <iostream>

using namespace oos;
using namespace std;

ObjectPrototypeTestUnit::ObjectPrototypeTestUnit()
  : unit_test("prototype", "ObjectStore Prototype Test Unit")
{
  add_test("empty", std::bind(&ObjectPrototypeTestUnit::empty_store, this), "test empty serializable store");
  add_test("find", std::bind(&ObjectPrototypeTestUnit::test_find, this), "find prototype test");
  add_test("size", std::bind(&ObjectPrototypeTestUnit::test_size, this), "size prototype test");
  add_test("parent_of", std::bind(&ObjectPrototypeTestUnit::test_is_parent_of, this), "check parent");
  add_test("decrement", std::bind(&ObjectPrototypeTestUnit::test_decrement, this), "check decrementing iterator");
  add_test("one", std::bind(&ObjectPrototypeTestUnit::one_prototype, this), "one prototype");
  add_test("hierarchy", std::bind(&ObjectPrototypeTestUnit::prototype_hierachy, this), "prototype hierarchy");
  add_test("iterator", std::bind(&ObjectPrototypeTestUnit::prototype_traverse, this), "prototype iterator");
  add_test("relation", std::bind(&ObjectPrototypeTestUnit::prototype_relation, this), "prototype relation");
}

ObjectPrototypeTestUnit::~ObjectPrototypeTestUnit()
{}

void
ObjectPrototypeTestUnit::initialize()
{}

void
ObjectPrototypeTestUnit::finalize()
{}

void
ObjectPrototypeTestUnit::empty_store()
{
  object_store ostore;

  UNIT_ASSERT_TRUE(ostore.empty(), "serializable store must be empty");
}

void
ObjectPrototypeTestUnit::test_find()
{
  object_store ostore;
  ostore.insert_prototype<Item>("item");
  
  prototype_iterator i = ostore.find_prototype<Item>();
  
  UNIT_ASSERT_TRUE(i != ostore.end(), "couldn't find prototype");
}

void
ObjectPrototypeTestUnit::test_size()
{
  object_store ostore;
  
  prototype_iterator i = ostore.begin();
  
  UNIT_ASSERT_TRUE(i->size() == 0, "prototype node must be empty");

  ostore.insert_prototype<Item>("item");
    
  i = ostore.find_prototype<Item>();

  UNIT_ASSERT_TRUE(i->size() == 0, "prototype node must be empty");
  
  ostore.insert(new Item);

  UNIT_ASSERT_TRUE(i->size() == 1, "prototype node must be one (1)");
}

void
ObjectPrototypeTestUnit::test_is_parent_of()
{
  object_store ostore;
  
  prototype_iterator root = ostore.begin();
  
  UNIT_ASSERT_TRUE(root->size() == 0, "prototype node must be empty");

  ostore.insert_prototype<Item>("item");
    
  prototype_iterator i = ostore.find_prototype<Item>();

  UNIT_ASSERT_FALSE(root->is_child_of(i.get()), "root must not be child of node");

//  UNIT_ASSERT_TRUE(i->is_child_of(root.get()), "node must be child of root");
}

void
ObjectPrototypeTestUnit::test_decrement()
{
  object_store ostore;
  ostore.insert_prototype<Item>("item");
  ostore.insert_prototype<ItemA, Item>("item_a", false);

  prototype_iterator i = ostore.end();

  --i;

  UNIT_ASSERT_TRUE(--i == ostore.begin(), "iterator must be begin");
}

void
ObjectPrototypeTestUnit::one_prototype()
{
  object_store ostore;

  ostore.insert_prototype<Item>("item");
  
  serializable *o = ostore.create("item");
  
  UNIT_ASSERT_NOT_NULL(o, "couldn't create serializable of type <Item>");
  
  Item *i = dynamic_cast<Item*>(o);
  
  UNIT_ASSERT_NOT_NULL(i, "couldn't cast serializable to Item");
  
  delete i;
  
  ostore.remove_prototype("item");
  
  UNIT_ASSERT_EXCEPTION(ostore.create("item"), object_exception, "unknown prototype type", "create with invalid type");
}

void
ObjectPrototypeTestUnit::prototype_hierachy()
{
  object_store ostore;
  ostore.insert_prototype<Item>("ITEM");
  ostore.insert_prototype<ItemA, Item>("ITEM_A");
  ostore.insert_prototype<ItemB, Item>("ITEM_B");
  ostore.insert_prototype<ItemC, Item>("ITEM_C");

  serializable *o = ostore.create("ITEM_B");
  
  UNIT_ASSERT_NOT_NULL(o, "couldn't create serializable of type <ItemB>");
  
  ItemB *a = dynamic_cast<ItemB*>(o);
  
  UNIT_ASSERT_NOT_NULL(a, "couldn't cast serializable to ItemB");
  
  delete a;
  
  ostore.remove_prototype("ITEM_B");
  
  UNIT_ASSERT_EXCEPTION(ostore.create("ITEM_B"), object_exception, "unknown prototype type", "create with invalid type");
  
  ostore.remove_prototype("ITEM");
  
  UNIT_ASSERT_EXCEPTION(ostore.create("ITEM"), object_exception, "unknown prototype type", "create with invalid type");
  UNIT_ASSERT_EXCEPTION(ostore.create("ITEM_A"), object_exception, "unknown prototype type", "create with invalid type");
  UNIT_ASSERT_EXCEPTION(ostore.create("ITEM_C"), object_exception, "unknown prototype type", "create with invalid type");
}

void
ObjectPrototypeTestUnit::prototype_traverse()
{
  object_store ostore;
  ostore.insert_prototype<Item>("ITEM");
  ostore.insert_prototype<ItemA, Item>("ITEM_A");
  ostore.insert_prototype<ItemB, Item>("ITEM_B");
  ostore.insert_prototype<ItemC, Item>("ITEM_C");

  prototype_iterator first = ostore.begin();
  prototype_iterator last = ostore.end();
  int count(0);

  while (first != last) {
    UNIT_ASSERT_LESS(count, 4, "prototype count isn't valid");
    ++first;
    ++count;
  }
  
  UNIT_ASSERT_EQUAL(count, 4, "expected prototype size isn't 4");
}

void
ObjectPrototypeTestUnit::prototype_relation()
{
  // Todo: complete test!
  object_store ostore;

  ostore.insert_prototype<album>("album");
  ostore.insert_prototype<track>("track");
  ostore.insert_prototype<playlist>("playlist");
}
