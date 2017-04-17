#include <iostream>
#include <system_error>

#include <cerrno>
#include <cstdlib>

#include "meevax/unix/shell.hpp"


int main(int argc, char** argv)
{
  unix::shell<char> sh {argc, argv};

  try
  {
    // sh.led(); // TODO signal handling
    while (true)
    {
      sh.read();
    }
  }

  catch (std::system_error& error)
  {
    std::cerr << "[error] code: " << error.code().value() << " - " << error.code().message() << std::endl;
    return error.code().value();
  }

  catch (...)
  {
    std::cerr << "[fatal] An unexpected error occurred. Report the following output to the developer.\n"
              << "\n"
              << "\tdeveloper's email: httperror@404-notfound.jp\n"
              << "\terrno: " << errno << " - " << std::strerror(errno) << std::endl;

    std::exit(errno);
  }
}
