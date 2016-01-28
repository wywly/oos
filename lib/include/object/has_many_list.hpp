//
// Created by sascha on 1/15/16.
//

#ifndef OOS_HAS_MANY_LIST_HPP
#define OOS_HAS_MANY_LIST_HPP

#include "object/basic_has_many.hpp"
#include "object/object_store.hpp"

#include <list>

namespace oos {

template < class T >
struct has_many_iterator_traits<T, std::list, typename std::enable_if<!std::is_scalar<T>::value>::type>
  : public std::iterator<std::bidirectional_iterator_tag, T>
{
  typedef has_many_iterator_traits<T, std::list> self;
  typedef object_ptr<T> value_type;
  typedef has_many_item<T> item_type;
  typedef has_one<item_type> internal_type;
  typedef object_ptr<item_type> relation_type;
  typedef std::list<internal_type, std::allocator<internal_type>> container_type;
  typedef typename container_type::iterator container_iterator;
  typedef typename std::iterator<std::bidirectional_iterator_tag, T>::difference_type difference_type;
};

template < class T >
struct has_many_iterator_traits<T, std::list, typename std::enable_if<std::is_scalar<T>::value>::type>
  : public std::iterator<std::bidirectional_iterator_tag, T>
{
  typedef has_many_iterator_traits<T, std::list> self;
  typedef T value_type;
  typedef has_many_item<T> item_type;
  typedef has_one<item_type> internal_type;
  typedef object_ptr<item_type> relation_type;
  typedef std::list<internal_type, std::allocator<internal_type>> container_type;
  typedef typename container_type::iterator container_iterator;
  typedef typename std::iterator<std::bidirectional_iterator_tag, T>::difference_type difference_type;
};

template < class T >
class has_many_iterator<T, std::list>
  : public has_many_iterator_traits<T, std::list>
{
public:
  typedef has_many_iterator_traits<T, std::list> traits;
  typedef has_many_iterator<T, std::list> self;
  typedef typename traits::value_type value_type;
  typedef typename traits::item_type item_type;
  typedef typename traits::internal_type internal_type;
  typedef typename traits::relation_type relation_type;
  typedef typename traits::container_type container_type;
  typedef typename traits::container_iterator container_iterator;
  typedef typename traits::difference_type difference_type;

public:
  has_many_iterator() {}
  explicit has_many_iterator(container_iterator iter)
    : iter_(iter)
  {}
  ~has_many_iterator() {}

  bool operator==(const self &i) const { return (iter_ == i.iter_); }
  bool operator!=(const self &i) const { return !this->operator==(i); }

  friend difference_type operator-(self a, self b) { return a.iter_ - b.iter_; }

  self& operator++()
  {
    ++iter_;
    return *this;
  }

  self operator++(int)
  {
    self tmp = *this;
    ++iter_;
    return tmp;
  }

  self& operator--()
  {
    self tmp = *this;
    --iter_;
    return tmp;
  }

  self operator--(int)
  {
    --iter_;
    return self();
  }

  value_type operator->() const { return get(); }
  value_type operator*() const { return get(); }
  value_type get() const { return (*iter_)->value(); }
  relation_type relation_item() const { return *iter_; }

private:
  friend class has_many<T, std::list>;
  friend class const_has_many_iterator<T, std::list>;
  friend class basic_has_many<T, std::list>;

  container_iterator iter_;
};

template < class T >
struct const_has_many_iterator_traits<T, std::list, typename std::enable_if<!std::is_scalar<T>::value>::type>
  : public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t, const T*, const T&>
{
  typedef has_many_iterator_traits<T, std::list> self;
  typedef object_ptr<T> value_type;
  typedef has_many_item<T> item_type;
  typedef has_one<item_type> internal_type;
  typedef object_ptr<item_type> relation_type;
  typedef std::list<internal_type, std::allocator<internal_type>> container_type;
  typedef typename container_type::iterator container_iterator;
  typedef typename std::iterator<std::bidirectional_iterator_tag, T>::difference_type difference_type;
};

template < class T >
struct const_has_many_iterator_traits<T, std::list, typename std::enable_if<std::is_scalar<T>::value>::type>
  : public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t, const T*, const T&>
{
  typedef has_many_iterator_traits<T, std::list> self;
  typedef T value_type;
  typedef has_many_item<T> item_type;
  typedef has_one<item_type> internal_type;
  typedef object_ptr<item_type> relation_type;
  typedef std::list<internal_type, std::allocator<internal_type>> container_type;
  typedef typename container_type::iterator container_iterator;
  typedef typename container_type::const_iterator const_container_iterator;
  typedef typename std::iterator<std::bidirectional_iterator_tag, T>::difference_type difference_type;
};


template < class T >
class const_has_many_iterator<T, std::list>
  : public const_has_many_iterator_traits<T, std::list>
{
public:
  typedef const_has_many_iterator_traits<T, std::list> traits;
  typedef const_has_many_iterator<T, std::list> self;
  typedef typename traits::value_type value_type;
  typedef typename traits::item_type item_type;
  typedef typename traits::internal_type internal_type;
  typedef typename traits::relation_type relation_type;
  typedef typename traits::difference_type difference_type;
  typedef typename traits::container_type container_type;
  typedef typename traits::container_iterator container_iterator;
  typedef typename traits::const_container_iterator const_container_iterator;

public:
  const_has_many_iterator() {}
  explicit const_has_many_iterator(container_iterator iter) : iter_(iter) {}
  explicit const_has_many_iterator(const_container_iterator iter) : iter_(iter) {}
  const_has_many_iterator(const has_many_iterator<T, std::list> &iter) : iter_(iter.iter_) {}
  const_has_many_iterator& operator=(const self &iter)
  {
    iter_ = iter.iter_;
    return *this;
  }
  const_has_many_iterator& operator=(const has_many_iterator<T, std::list> &iter)
  {
    iter_ = iter.iter_;
    return *this;
  }
  ~const_has_many_iterator() {}

  bool operator==(const self &i) const { return (iter_ == i.iter_); }
  bool operator!=(const self &i) const { return !this->operator==(i); }

  self& operator++()
  {
    ++iter_;
    return *this;
  }

  self operator++(int)
  {
    self tmp = *this;
    ++iter_;
    return tmp;
  }

  self& operator--()
  {
    --iter_;
    return *this;
  }

  self operator--(int)
  {
    self tmp = *this;
    --iter_;
    return tmp;
  }

  const value_type operator->() const { return get(); }
  const value_type operator*() const { return get(); }
  const value_type get() const { return (*iter_)->value(); }
  const relation_type relation_item() const { return *iter_; }

private:
  friend class has_many<T, std::list>;
  friend class basic_has_many<T, std::list>;

  const_container_iterator iter_;
};

template < class T >
class has_many<T, std::list> : public basic_has_many<T, std::list>
{
public:

  typedef basic_has_many<T, std::list> base;
  typedef typename base::iterator iterator;
  typedef typename base::value_type value_type;
  typedef typename base::item_type item_type;
  typedef typename base::relation_type relation_type;
  typedef typename base::container_iterator container_iterator;

  explicit has_many() {}

  iterator insert(iterator pos, const value_type &value)
  {
    // create new has_many
    item_type *item = this->create_item(value);
    relation_type iptr(item);
    if (this->ostore_) {
      this->ostore_->insert(iptr);
//      ostore_->mark_modified()
    }
    container_iterator i = pos.iter_;
    return iterator(this->container_.insert(i, iptr));
  }

  void push_front(const value_type &value)
  {
    insert(this->begin(), value);
  }

  void push_back(const value_type &value)
  {
    insert(this->end(), value);
  }

  iterator erase(iterator i)
  {
    return i;
  }
private:
  item_type* create_item(const value_type &value)
  {
    return new item_type(this->owner_field_, this->item_field_, this->owner_id_, value);
  }
};
//
//template < class T >
//class has_many<T, std::list, typename std::enable_if<std::is_scalar<T>::value>::type> : public basic_has_many<T, std::list>
//{
//public:
//
//  typedef basic_has_many<T, std::list> base;
//  typedef has_many_item<T> item_type;
//  typedef typename base::container_type container_type;
//  typedef typename base::iterator iterator;
//  typedef typename container_type::size_type size_type;
//  typedef typename container_type::iterator container_iterator;
//
//  explicit has_many() {}
//
//  iterator insert(iterator pos, T value)
//  {
//    item_type *item = this->create_item(value);
//    object_ptr<item_type> iptr(item);
//    if (this->ostore_) {
//      this->ostore_->insert(iptr);
////      ostore_->mark_modified()
//    }
//    container_iterator i = pos.iter_;
//    return iterator(this->container_.insert(i, iptr));
//  }
//
//  void push_front(T value)
//  {
//    // create new has_many
//    insert(this->begin(), value);
//  }
//
//  void push_back(T value)
//  {
//    insert(this->end(), value);
//  }
//
//private:
//  item_type* create_item(T value)
//  {
//    return new item_type(this->owner_field_, this->item_field_, this->owner_id_, value);
//  }
//};

}
#endif //OOS_HAS_MANY_LIST_HPP