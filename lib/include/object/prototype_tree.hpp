/*
 * This file is part of OpenObjectStore OOS.
 *
 * OpenObjectStore OOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenObjectStore OOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenObjectStore OOS. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PROTOTYPE_TREE_HPP
#define PROTOTYPE_TREE_HPP

#ifdef _MSC_VER
  #ifdef oos_EXPORTS
    #define OOS_API __declspec(dllexport)
    #define EXPIMP_TEMPLATE
  #else
    #define OOS_API __declspec(dllimport)
    #define EXPIMP_TEMPLATE extern
  #endif
  #pragma warning(disable: 4251)
#else
#define OOS_API
#define EXPIMP_TEMPLATE
#endif

#include "object/prototype_node.hpp"
#include "object/object_producer.hpp"

#include <string>
#include <unordered_map>

namespace oos {

class object_proxy;
class const_prototype_iterator;

/**
* @cond OOS_DEV
* @class prototype_iterator
* @brief An iterator for traversing the prototypes
*
* Defines an iterator for prototype_node. The nodes
* are stored in the object_store and must not be altered.
* therefor the iterator is declared as const.
*/
class OOS_API prototype_iterator : public std::iterator<std::bidirectional_iterator_tag, prototype_node, std::ptrdiff_t, prototype_node*, prototype_node&>
{
public:
  typedef std::iterator<std::bidirectional_iterator_tag, prototype_node, std::ptrdiff_t, prototype_node*, prototype_node&> base_iterator; /**< Shortcut to the base iterator class. */
  typedef prototype_iterator self;	            /**< Shortcut for this class. */
  typedef base_iterator::value_type value_type; /**< Shortcut for the value type. */
  typedef base_iterator::pointer pointer;       /**< Shortcut for the pointer type. */
  typedef base_iterator::reference reference ;  /**< Shortcut for the reference type */

  /**
  * Creates an empty iterator
  */
  prototype_iterator();

  /**
  * @brief Creates a iterator for a concrete type.
  *
  * This constructor creates an iterator for a concrete
  * type and a concrete serializable.
  *
  * @param node The prototype_node of the object_proxy
  */
  prototype_iterator(pointer node);

  /**
  * Copy from a given const_object_view_iterator.
  *
  * @param x The prototype_iterator to copy from.
  */
  prototype_iterator(const prototype_iterator &x);

  /**
  * Assign from a given prototype_iterator.
  *
  * @param x The prototype_iterator to assign from.
  * @return The assigned prototype_iterator.
  */
  prototype_iterator& operator=(const prototype_iterator &x);

  ~prototype_iterator();

  /**
  * @brief Compares this with another iterators.
  *
  * Compares this with another iterators. Returns true
  * if the iterators node prototype_type are the same.
  *
  * @param i The iterator to compare with.
  * @return True if the iterators are the same.
  */
  bool operator==(const prototype_iterator &i) const;
  bool operator==(const const_prototype_iterator &i) const;

  /**
  * @brief Compares this with another iterators.
  *
  * Compares this with another iterators. Returns true
  * if the iterators node prototype_node are not the same.
  *
  * @param i The iterator to compare with.
  * @return True if the iterators are not the same.
  */
  bool operator!=(const prototype_iterator &i) const;
  bool operator!=(const const_prototype_iterator &i) const;

  /**
  * Pre increments the iterator
  *
  * @return Returns iterators successor.
  */
  self& operator++();

  /**
  * Post increments the iterator
  *
  * @return Returns iterator before incrementing.
  */
  self operator++(int);

  /**
  * Pre increments the iterator
  *
  * @return Returns iterators predeccessor.
  */
  self& operator--();

  /**
  * Post decrements the iterator
  *
  * @return Returns iterator before decrementing.
  */
  self operator--(int);

  /**
  * Returns the pointer to the node.
  *
  * @return The pointer to the node.
  */
  pointer operator->() const;

  /**
  * Returns the node.
  *
  * @return The iterators underlaying node.
  */
  reference operator*() const;

  /**
  * Returns the pointer to the node.
  *
  * @return The pointer to the node.
  */
  pointer get() const;

private:
  void increment();
  void decrement();

private:
  friend class const_prototype_iterator;

  pointer node_;
};

class OOS_API const_prototype_iterator
  : public std::iterator<std::bidirectional_iterator_tag, prototype_node, std::ptrdiff_t, const prototype_node *, const prototype_node &>
{
public:
  typedef std::iterator<std::bidirectional_iterator_tag, prototype_node, std::ptrdiff_t, const prototype_node*, const prototype_node&> base_iterator; /**< Shortcut to the base iterator class. */
  typedef const_prototype_iterator self;	            /**< Shortcut for this class. */
  typedef base_iterator::value_type value_type; /**< Shortcut for the value type. */
  typedef base_iterator::pointer pointer;       /**< Shortcut for the pointer type. */
  typedef base_iterator::reference reference ;  /**< Shortcut for the reference type */

  /**
  * Creates an empty iterator
  */
  const_prototype_iterator();

  /**
  * @brief Creates a iterator for a concrete type.
  *
  * This constructor creates an iterator for a concrete
  * type and a concrete serializable.
  *
  * @param node The prototype_node of the object_proxy
  */
  const_prototype_iterator(pointer node);

  /**
  * Copy from a given const_object_view_iterator.
  *
  * @param x The prototype_iterator to copy from.
  */
  const_prototype_iterator(const const_prototype_iterator &x);

  /**
  * Copy from a given const_object_view_iterator.
  *
  * @param x The prototype_iterator to copy from.
  */
  const_prototype_iterator(const prototype_iterator &x);

  /**
  * Assign from a given prototype_iterator.
  *
  * @param x The prototype_iterator to assign from.
  * @return The assigned prototype_iterator.
  */
  const_prototype_iterator& operator=(const prototype_iterator &x);

  /**
  * Assign from a given prototype_iterator.
  *
  * @param x The prototype_iterator to assign from.
  * @return The assigned prototype_iterator.
  */
  const_prototype_iterator& operator=(const const_prototype_iterator &x);

  ~const_prototype_iterator();

  /**
  * @brief Compares this with another iterators.
  *
  * Compares this with another iterators. Returns true
  * if the iterators node prototype_type are the same.
  *
  * @param i The iterator to compare with.
  * @return True if the iterators are the same.
  */
  bool operator==(const const_prototype_iterator &i) const;

  /**
  * @brief Compares this with another iterators.
  *
  * Compares this with another iterators. Returns true
  * if the iterators node prototype_node are not the same.
  *
  * @param i The iterator to compare with.
  * @return True if the iterators are not the same.
  */
  bool operator!=(const const_prototype_iterator &i) const;

  /**
  * Pre increments the iterator
  *
  * @return Returns iterators successor.
  */
  self& operator++();

  /**
  * Post increments the iterator
  *
  * @return Returns iterator before incrementing.
  */
  self operator++(int);

  /**
  * Pre increments the iterator
  *
  * @return Returns iterators predeccessor.
  */
  self& operator--();

  /**
  * Post decrements the iterator
  *
  * @return Returns iterator before decrementing.
  */
  self operator--(int);

  /**
  * Returns the pointer to the node.
  *
  * @return The pointer to the node.
  */
  pointer operator->() const;

  /**
  * Returns the node.
  *
  * @return The iterators underlaying node.
  */
  reference operator*() const;

  /**
  * Returns the pointer to the node.
  *
  * @return The pointer to the node.
  */
  pointer get() const;

private:
  void increment();
  void decrement();

private:
  pointer node_;
};

/// @endcond

/**
 * @brief This class holds all prototypes nodes in a tree
 *
 * All prototype_node objects are stored within this tree. The user
 * decides the hierarchy when inserting a prototype_node
 *
 * The following example inserts a new prototype_node 'student' under
 * the base class 'person' (which must be already inserted).
 *
 * @code{.cpp}
 * tree.insert<student, person>("student");
 * @endcode
 *
 * One can find a prototype_node by its name or type
 *
 * @code{.cpp}
 * tree.find("student");
 * tree.find<student>();
 * @endcode
 *
 * One can also erase a prototype_node by its name or type
 *
 * @code{.cpp}
 * tree.remove("student");
 * tree.erase<student>();
 * @endcode
 *
 */
class OOS_API prototype_tree {
public:
  typedef prototype_iterator iterator;               /**< Shortcut for the list iterator. */
  typedef const_prototype_iterator const_iterator;   /**< Shortcut for the list const iterator. */

public:
  prototype_tree();
  ~prototype_tree();

  /**
  * Inserts a new serializable prototype into the prototype tree. The prototype
  * consists of a producer and a unique type name. To know where the new
  * prototype is inserted into the hierarchy the type name of the parent
  * node is also given.
  *
  * @param producer The producer serializable produces a new serializable of a specific type.
  * @param type     The unique name of the type.
  * @param abstract Indicates if the producers serializable is treated as an abstract node.
  * @param parent   The name of the parent type.
  * @return         Returns new inserted prototype iterator.
  */
  iterator insert(object_base_producer *producer, const char *type, bool abstract = false, const char *parent = nullptr);

  /**
  * Inserts a new serializable prototype into the prototype tree. The prototype
  * consists of a producer and a unique type name. To know where the new
  * prototype is inserted into the hierarchy the type name of the parent
  * node is also given. The producer is automatically created via the template
  * parameter.
  *
  * @tparam T       The type of the prototype node
  * @param type     The unique name of the type.
  * @param abstract Indicates if the producers serializable is treated as an abstract node.
  * @return         Returns new inserted prototype iterator.
  */
  template < class T >
  iterator insert(const char *type, bool abstract = false)
  {
    return insert(new object_producer<T>, type, abstract);
  }

  /**
  * Inserts a new serializable prototype into the prototype tree. The prototype
  * consists of a producer and a unique type name. To know where the new
  * prototype is inserted into the hierarchy the type name of the parent
  * node is also given. The producer is automatically created via the template
  * parameter.
  *
  * @tparam T       The type of the prototype node
  * @tparam S       The type of the parent prototype node
  * @param type     The unique name of the type.
  * @param abstract Indicates if the producers serializable is treated as an abstract node.
  * @return         Returns new inserted prototype iterator.
  */
  template < class T, class S >
  iterator insert(const char *type, bool abstract = false)
  {
    return insert(new object_producer<T>, type, abstract, typeid(S).name());
  }

  /**
  * @brief Finds prototype node.
  *
  * Finds and returns prototype node iterator identified
  * by the given name or classname (typeid). If the
  * prototype couldn't be found prototype_iterator end
  * is returned.
  *
  * @param type Name or class name of the prototype
  * @return Returns a prototype iterator.
  */
  iterator find(const char *type);

  /**
   * Prepares an insertion of a type
   *
   * @param type The name of the type to be prepared
   * @return The prepared prototype_node
   */
  prototype_node* prepare_insert(const char *type);

  /**
  * @brief Finds prototype node.
  *
  * Finds and returns prototype node const iterator identified
  * by the given name or classname (typeid). If the
  * prototype couldn't be found const_prototype_iterator end
  * is returned.
  *
  * @param type Name or class name of the prototype
  * @return Returns a prototype const iterator.
  */
  const_iterator find(const char *type) const;

  /**
  * @brief Finds prototype node by template type.
  * @tparam Template type.
  *
  * Finds and returns prototype node iterator identified
  * by the given template typeid. If the prototype couldn't
  * be found prototype_iterator end is returned.
  *
  * @return Returns a prototype iterator.
  */
  template < class T >
  iterator find()
  {
    return find(typeid(T).name());
  }

  /**
  * @brief Finds prototype node by template type.
  * @tparam Template type.
  *
  * Finds and returns prototype node const iterator identified
  * by the given template typeid. If the prototype couldn't
  * be found const_prototype_iterator end is returned.
  *
  * @return Returns a prototype const iterator.
  */
  template < class T >
  const_iterator find() const
  {
    return find(typeid(T).name());
  }

  /**
   * Returns true if prototype tree
   * is empty
   *
   * @return True on empty prototype tree
   */
  bool empty() const;

  /**
   * Return count of prototype nodes
   *
   * @return Count of prototype nodes
   */
  size_t size() const;

  /**
   * Return the number of prototypes
   * in the tree
   *
   * @return The number of prototypes
   */
  size_t prototype_count() const;

  /**
   * Clears the whole tree. All nodes and object_proxy objects
   * are deleted.
   */
  void clear();

  /**
   * Clears a prototype node and its cildren nodes.
   * All objects will be deleted.
   *
   * @param type The name of the type to remove.
   * @throws oos::object_exception on error
   */
  void clear(const char *type);

  /**
   * Clears a prototype node by an iterator and its
   * cildren nodes. All object_proxy objects will be
   * deleted.
   *
   * @param node The prototype_iterator to remove.
   * @throws oos::object_exception on error
   */
  void clear(const prototype_iterator &node);

  /**
   * Clears a prototype_node and its
   * cildren nodes. All object_proxy objects will be
   * deleted.
   *
   * @param node The prototype_node to remove.
   * @return The next valid prototype node.
   * @throws oos::object_exception on error
   */
  prototype_node* clear(prototype_node *node);

  /**
   * Determines the depth of a prototype_node.
   *
   * @param node The prototype_node requesting its depth
   * @return The depth of the given node
   */
  int depth(const prototype_node *node) const;

  /**
  * Dump all prototypes to a given stream
  *
  * @param out The stream to the prototypes dump on.
  */
  void dump(std::ostream &out) const;

  /**
   * Erase a prototype node identified
   * by its iterator. The successor iterator
   * is returned.
   *
   * @param i The prototype iterator to be erased
   * @return The successor of the erased iterator
   */
  iterator erase(const prototype_iterator &i);

  /**
  * Removes an serializable prototype from the prototype tree. All children
  * nodes and all objects are also removed.
  *
  * @param type The name of the type to remove.
  * @return Returns true if the type was found and successfully removed
  * @throws oos::object_exception on error
  */
  void remove(const char *type);

  /**
  * Removes an serializable prototype from the prototype tree. All children
  * nodes and all objects are also removed.
  *
  * @param node The prototype iterator node to remove.
  * @return Returns true if the type was found and successfully removed
  * @throws oos::object_exception on error
  */
  void remove(const prototype_iterator &node);

  /**
  * Return the first prototype node.
  *
  * @return The first prototype node iterator.
  */
  iterator begin();

  /**
  * Return the first prototype node.
  *
  * @return The first prototype node iterator.
  */
  const_iterator begin() const;

  /**
  * Return the last prototype node.
  *
  * @return The last prototype node iterator.
  */
  iterator end();

  /**
   * Return the last prototype node.
   *
   * @return The last prototype node iterator.
   */
  const_iterator end() const;

  /**
   * Executes a predicate for each root node
   *
   * @tparam P The type of the predicate function object
   * @param pred The function object to be executed
   */
  template < typename P >
  void for_each_root_node(P pred) const
  {
    prototype_node *node = first_->next;
    while (node != last_) {
      pred(const_prototype_iterator(node));
      node = node->next;
    }
  }
private:
  typedef std::unordered_map<std::string, prototype_node*> t_prototype_map;
  // typeid -> [name -> prototype]
  typedef std::unordered_map<std::string, t_prototype_map> t_typeid_prototype_map;

private:
  /**
   * @internal
   *
   * Searches a prototype by type
   * which can either be a type name
   * or a class name.
   * Returns a valid prototype node or
   * nullptr on unknown type
   * or throws an exception on error
   *
   * @param type Type name of the prototype node to search
   * @return The requested prototype node or nullptr
   * @throws oos::object_exception if in error occurrs
   */
  prototype_node* find_prototype_node(const char *type) const;

  /**
   * @internal
   *
   * Removes a prototy node and
   * return its successor node
   *
   * @param node The prototype node to remove
   * @param is_root Indicates if given node is root node
   * @return The successor node
   * @throws oos::object_exception if in error occurrs
   */
  prototype_node* remove_prototype_node(prototype_node *node, bool is_root);

private:
  /**
  * @internal
  *
  * Adjust first marker of all successor nodes with given serializable proxy.
  *
  * @param old_proxy The old first marker proxy.
  * @param new_proxy The new first marker proxy.
  */
  void adjust_right_marker(prototype_node *root, object_proxy *old_proxy, object_proxy *new_proxy);

  /**
  * @internal
  *
  * Adjusts self and last marker of all predeccessor nodes with given
  * serializable proxy.
  *
  * @param old_proxy The old last marker proxy.
  * @param new_proxy The new last marker proxy.
  */
  void adjust_left_marker(prototype_node *root, object_proxy *old_proxy, object_proxy *new_proxy);

private:
  /**
   * Get or create a prototype node
   *
   * @param producer The producer of the concrete object
   * @param type The type name of the node
   * @param abstract indicates wether the representing object is abstract
   * @return The prototype node
   */
  prototype_node *acquire(object_base_producer *producer, const char *type, bool abstract);

  /**
   * Initializes a prototype node
   *
   * @param node The node to initialize
   * @return iterator representing the prototype node
   */
  iterator initialize(prototype_node *node);

private:
  friend class object_container;
  friend class object_store;
  friend class prototype_node;
  friend class relation_resolver;

  prototype_node *first_;
  prototype_node *last_;

  // name to prototype node map
  t_prototype_map prototype_map_;
  // typeid to prototype node map
  t_typeid_prototype_map typeid_prototype_map_;
};

}

#endif /* PROTOTYPE_TREE_HPP */
