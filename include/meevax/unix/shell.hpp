#ifndef INCLUDED_MEEVAX_SHELL_HPP_
#define INCLUDED_MEEVAX_SHELL_HPP_


#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

#include <unistd.h>
#include <sys/wait.h>

#include "meevax/cmake_config.hpp"


namespace unix {


void execvpxx(const std::vector<std::string>& argv)
{
  std::vector<char*> args {};

  for (const auto& a : argv) args.emplace_back(const_cast<char*>(a.c_str()));
  args.push_back(nullptr);

  if (::execvp(args[0], args.data()) == -1)
  {
    throw std::system_error {errno, std::system_category()};
  }
}


int fork_exec(const std::vector<std::string>& args)
{
  switch (pid_t pid {::fork()})
  {
  case  0: // child process
    try
    {
      execvpxx(args);
    }
    catch (std::system_error error) // TODO error message
    {
      std::cout << "[error] code: " << error.code().value() << " - " << error.code().message() << std::endl;
      exit(EXIT_FAILURE);
    }

    break;

  case -1:
    std::cout << "[error] code: " << pid << " - " << ::strerror(errno);
    exit(EXIT_FAILURE);

    break;

  default:
    int status {};

    do {
      ::waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));

    break;
  }

  return 1;
}


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

  int run() // UGLY CODE !!!
  {
    std::cout << name_ << "$ ";

    std::vector<std::string> input_ {};

    for (std::string buffer; !std::getline(std::cin, buffer).eof(); input_.clear())
    {
      for (std::stringstream input {buffer};
           std::getline(input, buffer, ' ');
           input_.push_back(buffer));

      if (input_[0] == "exit")
      {
        std::cout << "[debug] exit called" << std::endl;
        return 0;
      }

      else if (input_[0] == "help")
      {
        std::cout << "[debug] help called" << std::endl;
        return 0;
      }

      else unix::fork_exec(input_);

      std::cout << name_ << "$ ";
    }

    return 0;
  }

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
