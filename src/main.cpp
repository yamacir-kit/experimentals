#include <algorithm>
#include <cmath>
#include <experimental/filesystem>
#include <iostream>
#include <iomanip>
#include <regex>
#include <string>
#include <vector>

#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <boost/asio.hpp>
#include <boost/cstdlib.hpp>

#include <meevax/ansi_escape_sequence/cursor.hpp>
#include <meevax/ansi_escape_sequence/graphics.hpp>
#include <meevax/configure/version.hpp>


namespace meevax::posix {


class termios
  : public ::termios
{
  const int fd_;
  struct ::termios default_;

public:
  explicit termios(int fd)
    : fd_ {::isatty(fd) ? fd : throw std::system_error {errno, std::system_category()}},
      default_ {(::tcgetattr(fd_, this), *this)}
  {}

  ~termios()
  {
    ::tcsetattr(fd_, TCSANOW, &default_);
  }

  decltype(auto) set(int optional_actions = TCSANOW) const noexcept
  {
    return ::tcsetattr(fd_, optional_actions, this);
  }
};


} // namespace meevax::posix


namespace meevax::ansi_escape_sequence::cursor {


class counted_newline_
{
  static signed int count_;

public:
  explicit counted_newline_() = default;

  auto countup() noexcept
  {
    return (++count_, newline);
  }

  auto restore() noexcept // XXX carriage return はコイツの仕事ではない
  {
    const std::string cursor_up {
      std::string {"\e["} + std::to_string(count_) + std::string {"A"}
    };

    count_ = 0;
    return cursor_up + "\r\e[K";
  }
} counted_newline;


signed int counted_newline_::count_ {0};


} // namespace meevax::ansi_escape_sequence::cursor


template <typename Char>
inline decltype(auto) operator<<(std::basic_ostream<Char>& lhs,
                                 meevax::ansi_escape_sequence::cursor::counted_newline_& rhs)
{
  return lhs << rhs.countup();
}


class window_size
  : public ::winsize
{
  const int fd_;

public:
  explicit window_size(int fd)
    : fd_ {::isatty(fd) ? fd : throw std::system_error {errno, std::system_category()}}
  {
    update();
  }

  void update() noexcept
  {
    ::ioctl(fd_, TIOCGWINSZ, this);
  }
};


auto main(int argc, char** argv) -> int try
{
  const std::vector<std::string> args {argv, argv + argc};

  for (auto iter {std::begin(args) + 1}; iter != std::end(args); ++iter) [&]()
  {
    for (const auto& option : decltype(args) {"^-v$", "^--version$"})
    {
      if (std::regex_match(*iter, std::regex {option}))
      {
        std::cout << meevax::project_name << " - version " << meevax::version;
        std::cout << " (Compiled " << meevax::build_time << " on " << meevax::build_type << " build)\n";

        std::exit(boost::exit_success);
      }
    }

    for (const auto& option : decltype(args) {"^-h$", "^--help$"})
    {
      if (std::regex_match(*iter, std::regex {option}))
      {
        std::cout << meevax::project_name << " - version " << meevax::version;
        std::cout << " (Compiled " << meevax::build_time << " on " << meevax::build_type << " build)\n";

        std::cout << "\n";
        std::cout << "usage: " << meevax::project_name << " [options]\n";
        std::cout << "       based on GNU command line interfaces standards\n";
        std::cout << "\n";
        std::cout << "options:\n";
        std::cout << "  -h, --help     display this help and exit\n";
        std::cout << "  -v, --version  output version information and exit\n";

        std::exit(boost::exit_success);
      }
    }

    std::cerr << "[error] unknown option \"" << *iter << "\"\n";
    std::cerr << "        more information with: \"" << meevax::project_name << " --help\"\n";

    std::exit(boost::exit_failure);
  }();


  static meevax::posix::termios termios {STDIN_FILENO};
  {
    termios.c_lflag &= ~(ICANON | ECHO);
    termios.c_cc[VMIN]  = 1;
    termios.c_cc[VTIME] = 0;

    termios.set();
  }


  while (true)
  {
    std::cout << meevax::ansi_escape_sequence::cursor::newline
              << meevax::ansi_escape_sequence::color::foreground::green
              << "meevax@"
              << boost::asio::ip::host_name()
              << ": "
              << meevax::ansi_escape_sequence::color::foreground::yellow
              << std::experimental::filesystem::current_path().string()
              << meevax::ansi_escape_sequence::color::foreground::white
              << "$ "
              << meevax::ansi_escape_sequence::attributes::off
              << meevax::ansi_escape_sequence::cursor::newline;

    for (std::vector<std::string> buffer {""}; false; )
    {
      std::size_t line {0}, column {0};

      const auto keypress {static_cast<char>(std::getchar())};
      switch (keypress)
      {
      case '\n':
        buffer.emplace_back("");
        break;

      default:
        buffer.back().push_back(keypress);
        break;
      }

      for (auto iter {std::begin(buffer)}; iter != std::end(buffer); ++iter)
      {
        if (buffer.size())
        {
        }

        auto digits = [](unsigned int num)
        {
          return num > 0 ? static_cast<int>(std::log(static_cast<double>(num))) + 1 : 1;
        };

        std::cout << std::setw(digits(buffer.size())) << std::right
                  << std::distance(std::begin(buffer), iter)
                  << *iter << "\n";
      }
    }
  }

  return boost::exit_success;
}

catch (const std::system_error& error)
{
  std::cerr << "[error] code: " << error.code().value()
            << " - " << error.code().message() << "\n";
  std::exit(error.code().value());
}

catch (const std::exception& error)
{
  std::cerr << "[error] " << error.what() << "\n";
  std::exit(boost::exit_exception_failure);
}

