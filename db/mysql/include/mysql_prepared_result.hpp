#ifndef MYSQL_PREPARED_RESULT_HPP
#define MYSQL_PREPARED_RESULT_HPP

#include "database/result_impl.hpp"

#ifdef WIN32
#include <winsock2.h>
#include <mysql.h>
#else
#include <mysql/mysql.h>
#endif

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

namespace oos {

class object_base_ptr;
class varchar_base;
class basic_identifier;

namespace mysql {

struct mysql_result_info;

class mysql_prepared_result : public detail::result_impl
{
private:
  mysql_prepared_result(const mysql_prepared_result &) = delete;
  mysql_prepared_result &operator=(const mysql_prepared_result &) = delete;

public:
  typedef detail::result_impl::size_type size_type;
  typedef std::unordered_map<std::string, std::shared_ptr<basic_identifier> > t_pk_map;

public:
  mysql_prepared_result(MYSQL_STMT *s, int rs, std::shared_ptr<oos::object_base_producer> producer);
  ~mysql_prepared_result();

  const char *column(size_type c) const;
  bool fetch();

  bool fetch(serializable *o);

  size_type affected_rows() const;
  size_type result_rows() const;
  size_type fields() const;

  virtual int transform_index(int index) const;

  virtual void read(const char *id, char &x);
  virtual void read(const char *id, short &x);
  virtual void read(const char *id, int &x);
  virtual void read(const char *id, long &x);
  virtual void read(const char *id, unsigned char &x);
  virtual void read(const char *id, unsigned short &x);
  virtual void read(const char *id, unsigned int &x);
  virtual void read(const char *id, unsigned long &x);
  virtual void read(const char *id, bool &x);
  virtual void read(const char *id, float &x);
  virtual void read(const char *id, double &x);
  virtual void read(const char *id, char *x, size_t s);
  virtual void read(const char *id, oos::date &x);
  virtual void read(const char *id, oos::time &x);
  virtual void read(const char *id, std::string &x);
  virtual void read(const char *id, varchar_base &x);
  virtual void read(const char *id, object_base_ptr &x);
  virtual void read(const char *id, object_container &x);
  virtual void read(const char *id, basic_identifier &x);

private:
  size_type affected_rows_;
  size_type rows;
  size_type fields_;
//  MYSQL_ROW row;
//  MYSQL_RES *res;
  MYSQL_STMT *stmt;
  int result_size;
  MYSQL_BIND *bind_;
  mysql_result_info *info_;

  t_pk_map pk_map_;
};

}

}

#endif /* MYSQL_PREPARED_RESULT_HPP */
