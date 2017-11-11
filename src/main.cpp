#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <experimental/filesystem>

#include <unistd.h>

#include <boost/asio.hpp>
#include <boost/cstdlib.hpp>

#include <meevax/ansi_escape_sequence/graphics_mode.hpp>
#include <meevax/configure/version.hpp>
#include <meevax/syntax/main.hpp>


int_main(const std::vector<std::string> args, [&]()
{
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

  // while (true)
  {
    std::cout << "\n"
              << meevax::ansi_escape_sequence::color::foreground::green
              << "meevax@"
              << boost::asio::ip::host_name()
              << ": "
              << meevax::ansi_escape_sequence::color::foreground::yellow
              << std::experimental::filesystem::current_path().string()
              << meevax::ansi_escape_sequence::color::foreground::white
              << "$ "
              << meevax::ansi_escape_sequence::attributes::off
              << "\n";
  }

  return boost::exit_success;
})

