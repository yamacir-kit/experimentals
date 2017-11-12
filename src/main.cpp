#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <experimental/filesystem>

#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <boost/asio.hpp>
#include <boost/cstdlib.hpp>

#include <meevax/ansi_escape_sequence/cursor.hpp>
#include <meevax/ansi_escape_sequence/graphics.hpp>
#include <meevax/configure/version.hpp>


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

