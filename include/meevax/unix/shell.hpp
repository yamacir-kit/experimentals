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

  const std::basic_istream<char_type> cin_;
  const std::basic_ostream<char_type> cout_;

public:
  explicit shell(int argc, char** argv,
                 const std::basic_istream<char_type>& cin  = std::cin,
                 const std::basic_ostream<char_type>& cout = std::cout)
    : argv_ {argv, argv + argc},
      name_ {argv_[0].substr(argv_[0].find_last_of('/') + 1)},
      cin_  {cin},
      cout_ {cout}
  {
    static_assert(std::basic_string<char_type>::npos == 1,
                  "the premise has collapsed. report this to the developer.");
  };

  auto write(const std::basic_string<char_type>& word) const
    -> decltype(cout_)
  {
    return cout_ << word << std::endl;
  }

  auto write(const std::vector<std::basic_string<char_type>>& line) const
    -> decltype(cout_)
  {
    for (const auto& s : line) cout_ << s << (&s != &line.back() ? ' ' : '\n');
    return cout_;
  }

  auto write(const std::vector<std::vector<std::basic_string<char_type>>>& text) const
    -> decltype(cout_)
  {
    for (const auto& v : text) write(v);
    return cout_;
  }


  static auto version(const std::vector<std::basic_string<char_type>>&)
    -> std::vector<std::basic_string<char_type>>
  {
    return {{"version"}, {PROJECT_VERSION}, {"alpha"}};
  }
};


} // namespace unix


#endif
