#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <experimental/filesystem>

#include <unistd.h>

#include <boost/asio.hpp>
#include <boost/cstdlib.hpp>

#include <meevax/configure/version.hpp>
#include <meevax/string/static_concat.hpp>
#include <meevax/syntax/main.hpp>


namespace meevax::ansi_escape_sequence::attributes {

static constexpr auto off        {meevax::string::static_concat("\e[0m")};
static constexpr auto bold       {meevax::string::static_concat("\e[1m")};
static constexpr auto unserscore {meevax::string::static_concat("\e[4m")};
static constexpr auto blink      {meevax::string::static_concat("\e[5m")};
static constexpr auto reverse    {meevax::string::static_concat("\e[7m")};
static constexpr auto concealed  {meevax::string::static_concat("\e[8m")};

} // meevax::ansi_escape_sequence::attributes


namespace meevax::ansi_escape_sequence::color::foreground {

static constexpr auto black   {meevax::string::static_concat("\e[30m")};
static constexpr auto red     {meevax::string::static_concat("\e[31m")};
static constexpr auto green   {meevax::string::static_concat("\e[32m")};
static constexpr auto yellow  {meevax::string::static_concat("\e[33m")};
static constexpr auto blue    {meevax::string::static_concat("\e[34m")};
static constexpr auto magenta {meevax::string::static_concat("\e[35m")};
static constexpr auto cyan    {meevax::string::static_concat("\e[36m")};
static constexpr auto white   {meevax::string::static_concat("\e[37m")};

} // meevax::ansi_escape_sequence::color::foreground


namespace meevax::ansi_escape_sequence::color::background {

static constexpr auto black   {meevax::string::static_concat("\e[40m")};
static constexpr auto red     {meevax::string::static_concat("\e[41m")};
static constexpr auto green   {meevax::string::static_concat("\e[42m")};
static constexpr auto yellow  {meevax::string::static_concat("\e[43m")};
static constexpr auto blue    {meevax::string::static_concat("\e[44m")};
static constexpr auto magenta {meevax::string::static_concat("\e[45m")};
static constexpr auto cyan    {meevax::string::static_concat("\e[46m")};
static constexpr auto white   {meevax::string::static_concat("\e[47m")};

} // meevax::ansi_escape_sequence::color::background


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

