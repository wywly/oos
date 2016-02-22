//
// Created by sascha on 9/7/15.
//

#include <sql/condition.hpp>
#include "SQLTestUnit.hpp"

#include "../Item.hpp"

#include "sql/statement.hpp"
#include "sql/query.hpp"

using namespace oos;

SQLTestUnit::SQLTestUnit(const std::string &name, const std::string &msg, const std::string &db)
  : unit_test(name, msg)
  , db_(db)
{
  add_test("create", std::bind(&SQLTestUnit::test_create, this), "test direct sql create statement");
  add_test("statement", std::bind(&SQLTestUnit::test_statement, this), "test prepared sql statement");
  add_test("foreign_query", std::bind(&SQLTestUnit::test_foreign_query, this), "test query with foreign key");
  add_test("query", std::bind(&SQLTestUnit::test_query, this), "test query");
  add_test("query_select", std::bind(&SQLTestUnit::test_query_select, this), "test query select");
}

SQLTestUnit::~SQLTestUnit() {}

void SQLTestUnit::initialize()
{
  connection_.reset(create_connection());
}

void SQLTestUnit::finalize() { }


void SQLTestUnit::test_create()
{
  connection_->open();

  query<Item> q(*connection_, "item");

  result<Item> res(q.create().execute());

  auto itime = oos::time(2015, 3, 15, 13, 56, 23, 123);
  Item hans("Hans", 4711);
  hans.set_time(itime);
  res = q.insert(&hans).execute();

  res = q.select().execute();

//  UNIT_ASSERT_EQUAL(res.size(), 1UL, "expected size must be one (1)");

  auto first = res.begin();
  auto last = res.end();

  while (first != last) {
    std::unique_ptr<Item> item(first.release());
    UNIT_ASSERT_EQUAL(item->get_string(), "Hans", "expected name must be 'Hans'");
    UNIT_ASSERT_EQUAL(item->get_int(), 4711, "expected integer must be 4711");
    UNIT_ASSERT_EQUAL(item->get_time(), itime, "expected time is invalid");
    ++first;
  }

  res = q.drop().execute();
}

void SQLTestUnit::test_statement()
{
  connection_->open();

  query<Item> q(*connection_, "item");

  statement<Item> stmt(q.create().prepare());

  result<Item> res(stmt.execute());

  oos::identifier<unsigned long> id(23);
  auto itime = oos::time(2015, 3, 15, 13, 56, 23, 123);
  Item hans("Hans", 4711);
  hans.id(id.value());
  hans.set_time(itime);
  stmt = q.insert(&hans).prepare();

  stmt.bind(&hans);
  res = stmt.execute();

  stmt = q.select().prepare();
  res = stmt.execute();

//  UNIT_ASSERT_EQUAL(res.size(), 1UL, "expected size must be one (1)");

  auto first = res.begin();
  auto last = res.end();

  while (first != last) {
    std::unique_ptr<Item> item(first.release());
    UNIT_ASSERT_EQUAL(item->id(), 23UL, "expected id must be 23");
    UNIT_ASSERT_EQUAL(item->get_string(), "Hans", "expected name must be 'Hans'");
    UNIT_ASSERT_EQUAL(item->get_int(), 4711, "expected integer must be 4711");
    UNIT_ASSERT_EQUAL(item->get_time(), itime, "expected time is invalid");
    ++first;
  }

  stmt = q.drop().prepare();

  res = stmt.execute();
}

void SQLTestUnit::test_foreign_query()
{
  connection_->open();

  query<Item> q(*connection_, "item");

  using t_object_item = ObjectItem<Item>;

  // create item table and insert item
  result<Item> res(q.create().execute());

  auto itime = oos::time(2015, 3, 15, 13, 56, 23, 123);
  oos::identifier<unsigned long> id(23);
  Item *hans = new Item("Hans", 4711);
  hans->id(id.value());
  hans->set_time(itime);
  res = q.insert(hans).execute();

  query<t_object_item> object_item_query(*connection_, "object_item");
  result<t_object_item> ores = object_item_query.create().execute();

  t_object_item oitem;
  oitem.ptr(hans);

  ores = object_item_query.insert(&oitem).execute();

  ores = object_item_query.select().execute();

  auto first = ores.begin();
  auto last = ores.end();

  while (first != last) {
    std::unique_ptr<t_object_item> obj(first.release());

    object_ptr<Item> i = obj->ptr();
    UNIT_ASSERT_TRUE(i.has_primary_key(), "expected valid identifier");
    UNIT_ASSERT_TRUE(*i.primary_key() == id, "expected id must be 23");

    ++first;
  }

  object_item_query.drop().execute();

  q.drop().execute();
}

void SQLTestUnit::test_query()
{
  connection_->open();

  query<person> q(*connection_, "person");

  // create item table and insert item
  result<person> res(q.create().execute());

  person hans("Hans", oos::date(12, 3, 1980), 180);
  hans.id(1);
  res = q.insert(&hans).execute();

  res = q.select().where("name='hans'").execute();

  auto first = res.begin();
  auto last = res.end();

  while (first != last) {
    std::unique_ptr<person> item(first.release());

    UNIT_ASSERT_EQUAL(item->name(), "Hans", "expected name must be 'Hans'");
    UNIT_ASSERT_EQUAL(item->height(), 180U, "expected height must be 180");
    UNIT_ASSERT_EQUAL(item->birthdate(), oos::date(12, 3, 1980), "expected birthdate is 12.3.1980");

    ++first;
  }

  res = q.select().where("name='heinz'").execute();

  UNIT_ASSERT_TRUE(res.begin() == res.end(), "begin must be equal end");

  q.drop().execute();
}

void SQLTestUnit::test_query_select()
{
  connection_->open();

  query<person> q(*connection_, "person");

  // create item table and insert item
  result<person> res(q.create().execute());

  unsigned long counter = 0;

  std::unique_ptr<person> hans(new person(++counter, "Hans", oos::date(12, 3, 1980), 180));
  res = q.insert(hans.get()).execute();

  std::unique_ptr<person> otto(new person(++counter, "Otto", oos::date(27, 11, 1954), 159));
  res = q.insert(otto.get()).execute();

  std::unique_ptr<person> hilde(new person(++counter, "Hilde", oos::date(13, 4, 1975), 175));
  res = q.insert(hilde.get()).execute();

  std::unique_ptr<person> trude(new person(++counter, "Trude", oos::date(1, 9, 1967), 166));
  res = q.insert(trude.get()).execute();

  res = q.select().execute();

//  UNIT_ASSERT_EQUAL(res.size(), 4UL, "result size must be one (4)");

  auto first = res.begin();
  auto last = res.end();

  while (first != last) {
    std::unique_ptr<person> item(first.release());
    ++first;
  }

  res = q.select().where("name='Hans'").execute();

//  UNIT_ASSERT_EQUAL(res.size(), 1UL, "result size must be one (1)");

  first = res.begin();
  last = res.end();

  while (first != last) {
    std::unique_ptr<person> item(first.release());

    UNIT_ASSERT_EQUAL(item->name(), "Hans", "expected name must be 'Hans'");
    UNIT_ASSERT_EQUAL(item->height(), 180U, "expected height must be 180");
    UNIT_ASSERT_EQUAL(item->birthdate(), oos::date(12, 3, 1980), "expected birthdate is 12.3.1980");

    ++first;
  }

  res = q.select().order_by("height").asc().execute();

  first = res.begin();

  bool is_first = true;
  while (first != last) {
    std::unique_ptr<person> item(first.release());

    if (is_first) {
      UNIT_ASSERT_EQUAL(item->name(), "Otto", "expected name must be 'Otto'");
      UNIT_ASSERT_EQUAL(item->height(), 159U, "expected height must be 159");
      is_first = false;
    }

    ++first;
  }

  res = q.select()
    .where("height>160")
    .and_(q.cond("height").less(180))
    .order_by("height")
    .desc()
    .execute();

//  UNIT_ASSERT_EQUAL(res.size(), 2UL, "result size must be one (2)");

  first = res.begin();

  is_first = true;
  while (first != last) {
    std::unique_ptr<person> item(first.release());

    if (is_first) {
      UNIT_ASSERT_EQUAL(item->name(), "Hilde", "expected name must be 'Hilde'");
      UNIT_ASSERT_EQUAL(item->height(), 175U, "expected height must be 175");
      is_first = false;
    }

    ++first;
  }

  res = q.drop().execute();
}

connection* SQLTestUnit::create_connection()
{
  return new connection(db_);
}

std::string SQLTestUnit::db() const {
  return db_;
}