#ifndef INCLUDED_MEEVAX_SYNTAX_MAIN_HPP
#define INCLUDED_MEEVAX_SYNTAX_MAIN_HPP


#include <iostream>
#include <stdexcept>
#include <system_error>

#include <boost/cstdlib.hpp>


#define int_main(args, lambda_expression, ...) \
  int main(int argc, char** argv) try                                          \
  {                                                                            \
    [[maybe_unused]] args {argv, argv + argc};                                 \
    return lambda_expression();                                                \
  }                                                                            \
                                                                               \
  catch (const std::system_error& error)                                       \
  {                                                                            \
    std::cerr << "[error] code: " << error.code().value()                      \
              << " - " << error.code().message() << "\n";                      \
    std::exit(error.code().value());                                           \
  }                                                                            \
                                                                               \
  catch (const std::exception& error)                                          \
  {                                                                            \
    std::cerr << "[error] " << error.what() << "\n";                           \
    std::exit(boost::exit_exception_failure);                                  \
  }                                                                            \
                                                                               \
  __VA_ARGS__                                                                  \


#endif

