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
  const             std::basic_string<char_type>  name_; // program called this class

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

  // auto& write(const std::basic_string<char_type>& word) const
  //   -> decltype(std::cout)
  // {
  //   return &(std::cout << word << std::endl);
  // }
  //
  // auto& write(const std::vector<std::basic_string<char_type>>& line) const
  //   -> decltype(std::cout)
  // {
  //   for (const auto& s : line) std::cout << s << (&s != &line.back() ? ' ' : '\n');
  //   return &std::cout;
  // }
  //
  // auto& write(const std::vector<std::vector<std::basic_string<char_type>>>& text) const
  //   -> decltype(std::cout)
  // {
  //   for (const auto& v : text) write(v);
  //   return &std::cout;
  // }


  static auto version(const std::vector<std::basic_string<char_type>>&)
    -> std::vector<std::basic_string<char_type>>
  {
    return {{"version"}, {PROJECT_VERSION}, {"alpha"}};
  }
};


} // namespace unix


#endif
