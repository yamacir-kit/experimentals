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

  for (std::string buffer {""}; std::cout << "\r\e[K>> " << buffer << std::flush; )
  {
    switch (auto key {meevax::operation::read()}; key)
    {
    case '\e':
      std::cout << "\r\e[K>> " << lisp::vectored_cons_cells {buffer} << " -> (quit)\r\n" << std::flush;
      std::exit(boost::exit_success);

    case ';':
      std::cout << "\r\e[K>> " << lisp::vectored_cons_cells {buffer} << " -> (execution)\r\n" << std::flush;
      std::cout << lisp::evaluate(buffer) << "\r\n\n" << std::flush;
      buffer.clear();
      break;

    default:
      buffer.push_back(key);
      break;
    }
  }

  return boost::exit_success;
}

