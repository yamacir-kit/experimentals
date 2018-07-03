#include <iostream>
#include <vector>

#include <boost/cstdlib.hpp>

// #include <meevax/iostream/read.hpp>
// #include <meevax/posix/termios.hpp>
// #include <meevax/syntax/lexer.hpp>

#include <corelisp/lisp/evaluator.hpp>


/**
* @file main.cpp
*
* @mainpage
* @section Overview
*/


auto main(int argc, char** argv) -> int
{
  const std::vector<std::string> args {argv, argv + argc};

  // meevax::posix::termios termios {meevax::posix::fd::stdin};
  // termios.change_to(meevax::posix::termios::input_mode::noncanonical);
  //
  // meevax::iostream::read read {std::cin};
  //
  // for (std::string buffer {""}; std::cin.good(); std::cout << std::string(64, '-') << "\r\n")
  // {
  //   using semantics = decltype(read)::hierarchical_semantics;
  //   if (const auto sequence {read.operator()<semantics::sequence>()}; sequence == "^C")
  //   {
  //     std::cout << "[debug] read sequence: ^C\n" << std::flush;
  //     return boost::exit_success;
  //   }
  //   else
  //   {
  //     buffer += sequence;
  //
  //     meevax::syntax::lexer_<char> lexer {};
  //
  //     std::cout << "source code: " << buffer << "\r\n";
  //     const auto translated {lexer.translate(buffer)};
  //     std::cout << "symbol expr: " << translated << "\r\n";
  //   }
  // }

  for (std::string buffer {}; std::cout << ">> ", std::getline(std::cin, buffer);)
  {
    std::cout << lisp::evaluate(buffer) << "\n\n";
  }

  return boost::exit_success;
}

