#include <iostream>
#include <system_error>

#include <cstdlib>

#include "meevax/unix/shell.hpp"


int main(int argc, char** argv)
{
  unix::shell<char> sh {argc, argv};

  try
  {
    sh.run(); // TODO signal handling
  }

  catch (std::system_error& error)
  {
    std::cerr << "[error] code: " << error.code().value() << " - " << error.code().message() << std::endl;
    return error.code().value();
  }

  catch (...)
  {
    std::cerr << "[fatal] an unexpected error occurred. report this to the developer.\n";
    std::exit(errno);
  }
}
