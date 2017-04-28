#include <iostream>
#include <system_error>

#include <cerrno>
#include <cstdlib>

#include <meevax/core/generic_editor.hpp>


int main(int argc, char** argv)
{
  unix::shell<char> sh {argc, argv};

  try
  {
    while (true)
    {
      sh.read();
      sh.write();
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
