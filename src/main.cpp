#include <regex>
#include <string>
#include <vector>

#include <boost/cstdlib.hpp>

#include <meevax/iostream/iostream.hpp>
#include <meevax/posix/posix.hpp>


/**
* @file main.cpp
*/


auto main(int argc, char** argv) -> int
{
  const std::vector<std::string> args {argv, argv + argc};

  meevax::posix::termios termios {meevax::posix::fd::stdin};
  termios.change_to(meevax::posix::termios::input_mode::noncanonical);

  meevax::iostream::read read {std::cin};
  for (std::string buffer {""}; std::cin.good(); std::cout << buffer << "\r\n" << std::flush)
  {
    using semantics = decltype(read)::hierarchical_semantics;
    if (const auto sequence {read.operator()<semantics::sequence>()}; sequence == "^C")
    {
      std::cout << "[debug] read sequence: ^C\n" << std::flush;
      return boost::exit_success;
    }
    else
    {
      buffer += sequence;
    }
  }

  return boost::exit_success;
}

