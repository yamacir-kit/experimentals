#ifndef INCLUDED_MEEVAX_SHELL_HPP_
#define INCLUDED_MEEVAX_SHELL_HPP_


#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "meevax/cmake_config.hpp"


namespace unix {


template <typename C>
class shell
{
public: // types
  using char_type = typename std::char_traits<C>::char_type;
  using size_type = typename std::basic_string<char_type>::size_type;

private: // internal data
  const std::vector<std::basic_string<char_type>> argv_;

public:
  explicit shell(int argc, char** argv)
    : argv_ {argv, argv + argc}
  {};

protected:
  static auto version(const line_type&) -> line_type
  {}
};


} // namespace unix


#endif
