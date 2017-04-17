#ifndef INCLUDED_MEEVAX_UNIX_SHELL_HPP_
#define INCLUDED_MEEVAX_UNIX_SHELL_HPP_


#include <iostream>
#include <regex>
#include <string>
#include <system_error>
#include <vector>

#include "meevax/metainfo.hpp"

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
  std::vector<std::vector<std::basic_string<char_type>>> text_buffer_;
              std::vector<std::basic_string<char_type>>  line_buffer_;
                          std::basic_string<char_type>   word_buffer_;
                                            char_type    char_buffer_;

  struct termios default_;

  static constexpr trial::static_concatenate<char_type> scat {};

public:
  explicit shell(int argc, char** argv)
    : text_buffer_ {},
      line_buffer_ {argv, argv + argc},
      word_buffer_ {},
      char_buffer_ {}
  {
    arguments_parse(line_buffer_);

    text_buffer_.push_back(line_buffer_);
    line_buffer_.clear();

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

  [[deprecated]] auto led()
  {
    while (true)
    {
      static constexpr auto color_green  {scat("\e[0;32m")};
      static constexpr auto color_yellow {scat("\e[0;33m")};
      static constexpr auto color_white  {scat("\e[0;37m")};

      static constexpr auto prompt {scat(color_green, "meevax@master-slave: ", color_yellow)};

      std::cout << prompt.data();
      std::cout << "(" << line_buffer_.size() + 1 << ": " << word_buffer_.size() << ") " << color_white.data();

      for (const auto& word : line_buffer_) { std::cout << word << "_"; };
      std::cout << word_buffer_ << std::endl;

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
    }
  }

  auto write() const
  {
  }

  auto read(decltype(word_buffer_)&& sequence = "")
  {
    switch (char_buffer_ = static_cast<decltype(char_buffer_)>(std::getchar()))
    {
      case 'a': std::cout << "[debug] a\n"; break;
      case 'b': std::cout << "[debug] b\n"; break;
      case 'c': std::cout << "[debug] c\n"; break;
      case 'd': std::cout << "[debug] d\n"; break;
      case 'e': std::cout << "[debug] e\n"; break;
      case 'f': std::cout << "[debug] f\n"; break;
      case 'g': std::cout << "[debug] g\n"; break;
      case 'h': std::cout << "[debug] h\n"; break;
      case 'i': std::cout << "[debug] i\n"; break;
      case 'j': std::cout << "[debug] j\n"; break;
      case 'k': std::cout << "[debug] k\n"; break;
      case 'l': std::cout << "[debug] l\n"; break;
      case 'm': std::cout << "[debug] m\n"; break;
      case 'n': std::cout << "[debug] n\n"; break;
      case 'o': std::cout << "[debug] o\n"; break;
      case 'p': std::cout << "[debug] p\n"; break;
      case 'q': std::cout << "[debug] q\n"; break;
      case 'r': std::cout << "[debug] r\n"; break;
      case 's': std::cout << "[debug] s\n"; break;
      case 't': std::cout << "[debug] t\n"; break;
      case 'u': std::cout << "[debug] u\n"; break;
      case 'v': std::cout << "[debug] v\n"; break;
      case 'w': std::cout << "[debug] w\n"; break;
      case 'x': std::cout << "[debug] x\n"; break;
      case 'y': std::cout << "[debug] y\n"; break;
      case 'z': std::cout << "[debug] z\n"; break;

      case 'A': std::cout << "[debug] A\n"; break;
      case 'B': std::cout << "[debug] B\n"; break;
      case 'C': std::cout << "[debug] C\n"; break;
      case 'D': std::cout << "[debug] D\n"; break;
      case 'E': std::cout << "[debug] E\n"; break;
      case 'F': std::cout << "[debug] F\n"; break;
      case 'G': std::cout << "[debug] G\n"; break;
      case 'H': std::cout << "[debug] H\n"; break;
      case 'I': std::cout << "[debug] I\n"; break;
      case 'J': std::cout << "[debug] J\n"; break;
      case 'K': std::cout << "[debug] K\n"; break;
      case 'L': std::cout << "[debug] L\n"; break;
      case 'M': std::cout << "[debug] M\n"; break;
      case 'N': std::cout << "[debug] N\n"; break;
      case 'O': std::cout << "[debug] O\n"; break;
      case 'P': std::cout << "[debug] P\n"; break;
      case 'Q': std::cout << "[debug] Q\n"; break;
      case 'R': std::cout << "[debug] R\n"; break;
      case 'S': std::cout << "[debug] S\n"; break;
      case 'T': std::cout << "[debug] T\n"; break;
      case 'U': std::cout << "[debug] U\n"; break;
      case 'V': std::cout << "[debug] V\n"; break;
      case 'W': std::cout << "[debug] W\n"; break;
      case 'X': std::cout << "[debug] X\n"; break;
      case 'Y': std::cout << "[debug] Y\n"; break;
      case 'Z': std::cout << "[debug] Z\n"; break;

      default:
        std::cout << "[debug] " << char_buffer_ << std::endl;
        break;
    }
  }

private:
  static auto arguments_parse(const decltype(line_buffer_)& argv)
  {
    for (auto iter {argv.begin()}; iter != argv.end(); ++iter)
    {
      for (const auto& s : decltype(argv) {"-h", "--help"})
      {
        if (std::regex_match(*iter, std::basic_regex<char_type> {s}))
        {
          help(argv);
          std::exit(0); // XXX DANGER CODE
        }
      }

      for (const auto& s : decltype(argv) {"-v", "--version"})
      {
        if (std::regex_match(*iter, std::basic_regex<char_type> {s}))
        {
          std::cout << version() << std::endl;
          std::exit(0); // XXX DANGER CODE
        }
      }
    }
  }

  static auto version()
  {
    static constexpr auto s {trial::static_concatenate<char_type>()("version ", PROJECT_VERSION, " alpha")};
    return s.data();
  }

  static void help(const decltype(line_buffer_)& argv)
  {
    std::cout << unix::basename(argv[0]) << " shell - " << version() << "\n\n"
              << "USAGE: " << unix::basename(argv[0]) << " [options]\n\n"
              << "\t-h, --help\tdisplay this help\n"
              << "\t-v, --version\tdisplay version information\n\n";
  }
};


} // namespace unix


#endif
