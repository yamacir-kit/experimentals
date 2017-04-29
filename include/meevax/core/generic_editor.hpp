#ifndef INCLUDED_MEEVAX_CORE_GENERIC_EDITOR_HPP_
#define INCLUDED_MEEVAX_CORE_GENERIC_EDITOR_HPP_


#include <iomanip>
#include <iostream>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include <meevax/version.hpp>
#include <meevax/joke/delayed_write.hpp>

#include <utilib/string/runtime_typename.hpp>
#include <utilib/string/static_concatenate.hpp>
#include <utilib/unix/basename.hpp>
#include <utilib/unix/execvp.hpp>
#include <utilib/unix/fork.hpp>

extern "C" {
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
}


namespace meevax {


template <typename C>
class generic_editor
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

  static constexpr utilib::static_concatenate<char_type> scat_ {};

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
  explicit generic_editor(int argc, char** argv)
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

  ~generic_editor()
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

    static constexpr auto remove_attributes {scat_("\e[0m")};

    static constexpr auto remove_line {scat_("\r", "\e[K")};

    static constexpr auto cursor_line        {scat_("\e[0;38;5;252m", "\e[48;5;236m")};
    static constexpr auto cursor_line_number {scat_("\e[1;38;5;221m", "\e[48;5;236m")};

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
  void semantic_parse()
  {
    if (parse_unit_ == semantic_parse_unit::line)
    {
      std::basic_stringstream<char_type> bssc {};

      bssc << "\n\n";
      bssc << "[parse] target: unnamed buffer line " << cursor_.first << " (";

      for (const auto& word : text_buffer_[cursor_.first])
      {
        bssc << word << (&word != &text_buffer_[cursor_.first].back() ? " " : ")\n");
      }

      bssc << "\tsyntax: external commands\n";
      bssc << "\tparser: execvp(3)\n";
      bssc << "\tstructure: " << typeid(text_buffer_[cursor_.first]).name() << '\n';
      bssc << '\n';

      meevax::delayed_incremental_write(bssc);
      // meevax::delayed_write(bssc);

      unix::fork()(unix::execvp<char_type>(text_buffer_[cursor_.first++]));

      std::cout << "\n";
    }
  }

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
    static constexpr auto s {scat_("version ", project_version, " alpha")};
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


} // namespace meevax


#endif
