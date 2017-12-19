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
// #include <meevax/posix/inline_curses.hpp>
#include <meevax/posix/termios.hpp>
#include <meevax/posix/winsize.hpp>

// #include <meevax/system/io_system.hpp>
#include <meevax/semantics/r.hpp>


auto main(int argc, char** argv) -> int try
{
  const std::vector<std::string> args {argv, argv + argc};

  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

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

  while (true)
  {
    static std::string buffer {};

    switch (const auto code {meevax::semantics::r<char>(std::cin)}; code)
    {
    case 'w':
      {
        std::cout << "[debug] code: " << code << "\n";
        std::cout << buffer << std::endl;
      }
      break;

    default:
      {
        std::cout << "[debug] code: " << code << "\n" << std::flush;
        buffer.push_back(code);
      }
      break;
    }
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

