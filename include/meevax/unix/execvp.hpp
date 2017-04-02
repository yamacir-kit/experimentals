#ifndef INCLUDED_MEEVAX_UNIX_EXECVP_HPP_
#define INCLUDED_MEEVAX_UNIX_EXECVP_HPP_


#include <algorithm>
#include <string>
#include <system_error>
#include <vector>

#include <unistd.h>


namespace unix {


template <typename C>
class execvp
{
  std::vector<C*> argv_;

public:
  execvp(const std::vector<std::basic_string<C>>& argv)
    : argv_ {}
  {
    for (const auto& v : argv) argv_.emplace_back(const_cast<C*>(v.c_str()));
    argv_.push_back(nullptr);
  }

  void operator()()
  {
    if (::execvp(argv_[0], argv_.data()) == -1)
    {
      throw std::system_error {errno, std::system_category()};
    }
  }
};


} // namespace unix


#endif
