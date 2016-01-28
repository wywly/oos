#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "sql/result.hpp"
#include "sql/connection_impl.hpp"

#include <string>

namespace oos {

class connection
{
public:
  explicit connection(const std::string &dns);

  template < class T >
  result<T> execute(const std::string &stmt)
  {
    return result<T>(impl_->execute(stmt));
  }

private:
  std::string type_;
  std::string dns_;
  connection_impl *impl_;
};

}
#endif /* CONNECTION_HPP */