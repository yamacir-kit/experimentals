#include <vector>

#include <boost/cstdlib.hpp>

#include <meevax/iostream/iostream.hpp>
#include <meevax/posix/posix.hpp>
#include <meevax/syntax/syntax.hpp>


/**
* @file main.cpp
*/


auto main(int argc, char** argv) -> int
{
  const std::vector<std::string> args {argv, argv + argc};

  meevax::posix::termios termios {meevax::posix::fd::stdin};
  termios.change_to(meevax::posix::termios::input_mode::noncanonical);

  meevax::iostream::read read {std::cin};
  for (std::string buffer {""}; std::cin.good(); std::cout << std::string(64, '-') << "\r\n")
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

      meevax::syntax::lexer<char> lexer {};

      std::cout << "[debug]  raw code: " << buffer << "\r\n";
      std::cout << "[debug] tokenized: " << lexer.tokenize(buffer) << "\r\n";

      for (const auto& each_map : lexer.extracted)
      {
        std::size_t index {0};
        for (const auto& parts : each_map.second)
        {
          std::cout << std::string(8, ' ') << "(" << each_map.first << ")[" << index++ << "] " << parts << "\r\n";
        }
      }
    }
  }

  return boost::exit_success;
}

