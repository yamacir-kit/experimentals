#ifndef INCLUDED_MEEVAX_UNIX_SHELL_HPP_
#define INCLUDED_MEEVAX_UNIX_SHELL_HPP_


#include <iostream>
#include <regex>
#include <string>
#include <system_error>
#include <vector>

#include "meevax/cmake_config.hpp"

#include "meevax/unix/basename.hpp"
#include "meevax/unix/execvp.hpp"
#include "meevax/unix/fork.hpp"

#include "meevax/trial/static_concatenate.hpp"

extern "C" {
#include <termios.h>
#include <unistd.h>
}


namespace unix {


template <typename C>
class shell
{
public:
  using char_type = typename std::basic_string<C>::value_type;
  using size_type = typename std::basic_string<char_type>::size_type;

private:
  const std::vector<std::basic_string<char_type>> argv_;

  std::vector<std::vector<std::basic_string<char_type>>> text_buffer_;
              std::vector<std::basic_string<char_type>>  line_buffer_;
                          std::basic_string<char_type>   word_buffer_;
                                            char_type    char_buffer_;

  struct termios default_;

public:
  explicit shell(int argc, char** argv)
    : argv_ {argv, argv + argc}
  {
    for (auto iter {argv_.begin()}; iter != argv_.end(); ++iter)
    {
      for (const auto& s : std::vector<std::basic_string<char_type>> {"-h", "--help"})
      {
        if (std::regex_match(*iter, std::basic_regex<char_type>{s}))
        {
          help();
          std::exit(0); // XXX DANGER CODE
        }
      }

      for (const auto& s : std::vector<std::basic_string<char_type>> {"-v", "--version"})
      {
        if (std::regex_match(*iter, std::basic_regex<char_type> {s}))
        {
          std::cout << version() << std::endl;
          std::exit(0); // XXX DANGER CODE
        }
      }
    }

    ::tcgetattr(STDIN_FILENO, &default_);

    struct termios ios {default_};
    ios.c_lflag &= ~(ICANON | ECHO);

    ios.c_cc[VMIN]  = 1;
    ios.c_cc[VTIME] = 0;

    ::tcsetattr(STDIN_FILENO, TCSANOW, &ios);
  };

  ~shell()
  {
    ::tcsetattr(STDIN_FILENO, TCSANOW, &default_);
  }

  auto led()
  {
    // std::basic_string<char_type> word_buffer {};
    // std::vector<decltype(word_buffer)> line_buffer {};

    while (true)
    {
      // typename decltype(word_buffer)::value_type char_buffer {};
      ::read(STDIN_FILENO, &char_buffer_, sizeof(decltype(char_buffer_)));

      switch (char_buffer_)
      {
        case ' ':
          line_buffer_.push_back(word_buffer_);
          word_buffer_.clear();
          break;

        case '\n':
          if (word_buffer_.size() > 0)
          {
            line_buffer_.push_back(word_buffer_);
            word_buffer_.clear();
          }
          // return line_buffer_;

          unix::fork()(unix::execvp<char_type>(line_buffer_));
          line_buffer_.clear();
          word_buffer_.clear();

          break;

        case 127:
          if (word_buffer_.size() > 0) { word_buffer_.pop_back(); }

          else if (line_buffer_.size() > 0)
          {
            word_buffer_ = line_buffer_.back();
            line_buffer_.pop_back();
          }

          break;

        default:
          if (std::isgraph(char_buffer_))
          {
            word_buffer_.push_back(char_buffer_);
          }
          break;
      }

      std::cout << "[debug] (" << line_buffer_.size() + 1 << ": " << word_buffer_.size() << ") ";
      for (const auto& word : line_buffer_) { std::cout << word << "_"; };
      std::cout << word_buffer_ << std::endl;
    }
  }

private:
  static auto version()
  {
    static constexpr auto s {trial::static_concatenate<char_type>()("version ", PROJECT_VERSION, " alpha")};
    return s.data();
  }

  void help() const
  {
    std::cout << unix::basename(argv_[0]) << " shell - " << version() << "\n\n"
              << "USAGE: " << unix::basename(argv_[0]) << " [options]\n\n"
              << "\t-h, --help\tdisplay this help\n"
              << "\t-v, --version\tdisplay version information\n\n";
  }
};


} // namespace unix


#endif
