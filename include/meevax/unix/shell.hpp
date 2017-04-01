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
  const             std::basic_string<char_type>  name_; // TODO function basename

public:
  explicit shell(int argc, char** argv)
    : argv_ {argv, argv + argc},
      name_ {argv_[0].substr(argv_[0].find_last_of('/') + 1)}
  {
    static_assert(std::basic_string<char_type>::npos == -1,
                  "the premise has collapsed. report this to the developer.");

    for (const auto& v : help(argv_))
      for (const auto& s : v) std::cout << s << (&s != &v.back() ? ' ' : '\n');
  };

  static auto version(const std::vector<std::basic_string<char_type>>&)
    -> std::vector<std::basic_string<char_type>>
  {
    return {{"version"}, {PROJECT_VERSION}, {"alpha"}};
  }

  auto help(const std::vector<std::basic_string<char_type>>&) // UGLY CODE !!!
    -> std::vector<std::vector<std::basic_string<char_type>>>
  {
    return {
      {name_, {"shell"}, {"-"}, {"the most modern guardian of CUI culture."}}, // TODO function basename
      {{}},
      version(argv_),
      {{}},
      {{"USAGE:"}, name_, {"[options]"}},
      {{}},
      {{"\t"}, {"-h"}, {"--help"},    {"\t"}, {"display this help"}},
      {{"\t"}, {"-v"}, {"--version"}, {"\t"}, {"display version information"}}
    };
  }
};


} // namespace unix


#endif
