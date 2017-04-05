#ifndef INCLUDED_MEEVAX_UNIX_SHELL_HPP_
#define INCLUDED_MEEVAX_UNIX_SHELL_HPP_


#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <system_error>
#include <vector>

#include "meevax/cmake_config.hpp"
#include "meevax/unix/execvp.hpp"
#include "meevax/unix/fork.hpp"


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

        std::vector<std::basic_string<char_type>> input_;

public:
  explicit shell(int argc, char** argv)
    : argv_ {argv, argv + argc},
      name_ {argv_[0].substr(argv_[0].find_last_of('/') + 1)},
      input_ {}
  {
    static_assert(std::basic_string<char_type>::npos == -1,
                  "the premise has collapsed. report this to the developer.");

    for (auto iter {argv_.begin()}; iter != argv_.end(); ++iter)
    {
      for (const auto& s : std::vector<std::basic_string<char_type>>{"-h", "--help"})
      {
        if (std::regex_match(*iter, std::basic_regex<char_type>{s}))
        {
          for (const auto& v : help(argv_))
            for (const auto& s : v) std::cout << s << (&s != &v.back() ? ' ' : '\n');
        }
      }

      for (const auto& s : std::vector<std::basic_string<char_type>>{"-v", "--version"})
      {
        if (std::regex_match(*iter, std::basic_regex<char_type>{s}))
        {
          for (const auto& s : version(argv_)) std::cout << s << ' ';
          std::cout << std::endl;
        }
      }
    }
  };

  int run() // XXX UGLY CODE !!!
  {
    std::cout << name_ << "$ ";

    for (std::string buffer; !std::getline(std::cin, buffer).eof(); input_.clear())
    {
      for (std::stringstream input {buffer};
           std::getline(input, buffer, ' ');
           input_.push_back(buffer));

      if (input_[0] == "exit") { return 0; }

      else if (input_[0] == "help")
      {
        std::cout << "[debug] help called" << std::endl;
        return 0;
      }

      try { unix::fork()(unix::execvp<char_type>(input_)); }

      catch (std::system_error&) { throw; }

      catch (...) { throw; }

      std::cout << name_ << "$ ";
    }

    return 0;
  }

  const auto input() const noexcept { return input_; }

protected:
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
