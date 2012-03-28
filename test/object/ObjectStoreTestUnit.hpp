#ifndef OBJECTSTORE_TESTUNIT_HPP
#define OBJECTSTORE_TESTUNIT_HPP

#include "unit/unit_test.hpp"

#include "tools/varchar.hpp"

#include "object/object.hpp"
#include "object/object_ptr.hpp"
#include "object/object_store.hpp"
#include "object/object_atomizer.hpp"

class ObjectStoreTestUnit : public oos::unit_test
{
private:
  class SimpleObject : public oos::object
  {
  public:
    SimpleObject()
      : number_(0)
    {}
    SimpleObject(const std::string &n)
      : number_(0)
      , name_(n)
    {}
    SimpleObject(int n)
      : number_(n)
    {}
    SimpleObject(const std::string &n, int i)
      : number_(i)
      , name_(n)
    {}
    virtual ~SimpleObject() {}
    
    void read_from(oos::object_atomizer *reader)
    {
      object::read_from(reader);
      reader->read_int("number", number_);
      reader->read_varchar("name", name_);
      reader->read_string("text", text_);
    }
    void write_to(oos::object_atomizer *writer) const
    {
      object::write_to(writer);
      writer->write_int("number", number_);
      writer->write_varchar("name", name_);
      writer->write_string("text", text_);
    }

    int number() const { return number_; }
    void number(int n) { number_ = n; }

    std::string name() const { return name_.str(); }
    void name(const std::string &n) { name_ = n; }

    std::string text() const { return text_; }
    void text(const std::string &t) { text_ = t; }

  private:
    int number_;
    oos::varchar<32> name_;
    std::string text_;
  };
  class ObjectWithSubObject : public oos::object
  {
  public:
    ObjectWithSubObject() {}
    virtual ~ObjectWithSubObject() {}

    void read_from(oos::object_atomizer *reader)
    {
      object::read_from(reader);
      reader->read_object("simple", simple_);
    }
    void write_to(oos::object_atomizer *writer) const
    {
      object::write_to(writer);
      writer->write_object("simple", simple_);
    }

    oos::object_ptr<SimpleObject> simple() const { return simple_; }
    void simple(const oos::object_ptr<SimpleObject> &s) { simple_ = s; }

  private:
    oos::object_ptr<SimpleObject> simple_;
  };

public:
  ObjectStoreTestUnit();
  virtual ~ObjectStoreTestUnit();
  
  virtual void initialize();
  virtual void finalize();
  
  void expression_test();
  void access_value();
  void serializer();
  void ref_ptr_counter();
  void simple_object();
  void object_with_sub_object();
  void multiple_simple_objects();
  void multiple_object_with_sub_objects();
  void delete_object();
  void sub_delete();
  void hierarchy();

private:
  oos::object_store ostore_;
};

#endif /* OBJECTSTORE_TESTUNIT_HPP */
