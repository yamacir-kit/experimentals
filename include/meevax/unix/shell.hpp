#ifndef INCLUDED_MEEVAX_UNIX_SHELL_HPP_
#define INCLUDED_MEEVAX_UNIX_SHELL_HPP_


#include <iostream>
#include <iomanip>
#include <regex>
#include <string>
#include <system_error>
#include <vector>
#include <utility>

#include <meevax/version.hpp>

#include "meevax/unix/basename.hpp"
#include "meevax/unix/execvp.hpp"
#include "meevax/unix/fork.hpp"

#include "meevax/trial/static_concatenate.hpp"

extern "C" {
#include <sys/ioctl.h>
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

  const std::vector<std::basic_string<char_type>> mode_message_ {
    {"text"}, {"line"}, {"word"}
  };

  enum class semantic_parse_unit
    : typename decltype(mode_message_)::size_type
  {
    text, line, word,
  } parse_unit_;

  std::pair<std::size_t, std::size_t> cursor_;

public:
  explicit shell(int argc, char** argv)
    : text_buffer_ {},
      line_buffer_ {},
      word_buffer_ {},
      char_buffer_ {},
      parse_unit_  {semantic_parse_unit::line},
      cursor_ {0, 0}
  {
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

  auto write() const
  {
    // std::cout << "[semantic_parse_unit: "
    //           << mode_message_[static_cast<typename std::underlying_type<decltype(parse_unit_)>::type>(parse_unit_)]
    //           << "] ";

    // for (const auto& line : text_buffer_)
    // {
    //   for (const auto& word : line)
    //   {
    //     std::cout << word << (&word != &line.back() ? " " : "\e[0;33m\\n\n\e[0;37m");
    //   }
    // }

    struct winsize window_size {};
    ::ioctl(STDOUT_FILENO, TIOCGWINSZ, &window_size);

    static constexpr auto remove_attributes {scat("\e[0m")};

    static constexpr auto remove_line {scat("\r", "\e[K")};

    static constexpr auto cursor_line        {scat("\e[0;38;5;252m", "\e[48;5;236m")};
    static constexpr auto cursor_line_number {scat("\e[1;38;5;221m", "\e[48;5;236m")};

    std::size_t digits {};
    for (auto size {text_buffer_.size() + 1}; size /= 10; ++digits);

    std::cout << remove_line.data()
              << cursor_line.data();

    for (decltype(window_size.ws_col) col {}; col < window_size.ws_col; col++)
    {
      std::cout << " ";
    }

    std::cout << "\r";

    std::cout << cursor_line_number.data() << std::setw(digits + 2) << cursor_.first;
    std::cout << cursor_line.data() << " $ ";

    for (const auto& word : line_buffer_)
    {
      std::cout << word << " ";
    }

    std::cout << word_buffer_ << remove_attributes.data();
  }

  auto read(decltype(word_buffer_)&& forwarded = "") // XXX HARD CODING !!!
  {
    switch (char_buffer_ = static_cast<decltype(char_buffer_)>(std::getchar()))
    {
#undef  MEEVAX_DEBUG_KEYBIND
// #define MEEVAX_DEBUG_KEYBIND
#include <meevax/master-slave/ansi_escape_sequences.cpp>
#undef  MEEVAX_DEBUG_KEYBIND
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
