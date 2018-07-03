/**
* @file main.cpp
*
* @mainpage
* @section Overview
*/


#include <iostream>
#include <vector>

#include <boost/cstdlib.hpp>

#include <corelisp/lisp/evaluator.hpp>
#include <corelisp/lisp/tokenizer.hpp>
#include <corelisp/lisp/vectored_cons_cells.hpp>

#include <meevax/operation/reader.hpp>


auto main(int argc, char** argv) -> int
{
  const std::vector<std::string> args {argv, argv + argc};

  std::cin.tie(0);
  std::ios::sync_with_stdio(false);

  for (std::string buffer {}; std::cout << "\r\n\n>> "; )
  {
    if (auto key {meevax::operation::read()}; std::isgraph(key))
    {
      buffer.push_back(key);
    }

    switch (buffer.back())
    {
    case 'q':
      std::exit(boost::exit_success);

    case 'e':
      std::cout << lisp::evaluate(buffer) << std::flush;
      buffer.clear();
      break;

    default:
      std::cout << buffer << std::flush;
      break;
    }
  }

  return boost::exit_success;
}

