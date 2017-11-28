#include <exception>
#include <experimental/filesystem>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/cstdlib.hpp>

#include <meevax/ansi_escape_sequence/cursor.hpp>
#include <meevax/ansi_escape_sequence/graphics.hpp>
#include <meevax/configure/version.hpp>
#include <meevax/posix/inline_curses.hpp>
#include <meevax/posix/termios.hpp>
#include <meevax/posix/winsize.hpp>


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
  termios.change_to_noncanonical_mode();

  static meevax::posix::winsize winsize {STDIN_FILENO};

  static meevax::posix::inline_curses<char> icurses {std::cin, std::cout, winsize};

  while (true)
  {
    icurses.read();
    icurses.write();
  }

  return boost::exit_success;
}

catch (const std::system_error& error)
{
  const auto code {error.code()};
  std::cerr << "\n[error] code: " << code.value() << " - " << code.message() << "\n";
  std::exit(error.code().value());
}

catch (const std::exception& error)
{
  std::cerr << "\n[error] " << error.what() << "\n";
  std::exit(boost::exit_exception_failure);
}

