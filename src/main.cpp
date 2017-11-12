#include <algorithm>
#include <cmath>
#include <experimental/filesystem>
#include <iostream>
#include <iomanip>
#include <regex>
#include <string>
#include <vector>

#include <sys/ioctl.h>
#include <unistd.h>

#include <boost/asio.hpp>
#include <boost/cstdlib.hpp>

#include <meevax/ansi_escape_sequence/cursor.hpp>
#include <meevax/ansi_escape_sequence/graphics.hpp>
#include <meevax/configure/version.hpp>
#include <meevax/posix/termios.hpp>


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

