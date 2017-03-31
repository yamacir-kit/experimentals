#ifndef INCLUDED_MEEVAX_SHELL_HPP_
#define INCLUDED_MEEVAX_SHELL_HPP_


#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "meevax/cmake_config.hpp"


namespace unix {


template <typename C, typename Tr = std::char_traits<C>>
class shell
{
  const std::vector<std::basic_string<C>> argv_;

public:
  using char_type = typename Tr::char_type;
  using size_type = std::size_t;

  using word_type = std::basic_string<char_type>;
  using line_type = std::vector<word_type>;
  using text_type = std::vector<line_type>;

  explicit shell(int argc, char** argv)
    : argv_ {argv, argv + argc}
  {};

protected:
  static auto version(const line_type&) -> line_type
  {}
};


} // namespace unix


#endif
