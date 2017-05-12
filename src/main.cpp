#include <iostream>
#include <system_error>

#include <cerrno>
#include <cstdlib>

#include <meevax/core/generic_editor.hpp>

#include <meevax/visual/visual_stream.hpp>
#include <meevax/visual/visual_manipulator.hpp>


int main(int argc, char** argv)
{
  // meevax::generic_editor<char> editor {argc, argv};
  //
  // try
  // {
  //   while (true)
  //   {
  //     editor.write();
  //     editor.read();
  //   }
  // }
  //
  // catch (std::system_error& error)
  // {
  //   std::cerr << "[error] code: " << error.code().value() << " - " << error.code().message() << std::endl;
  //   return error.code().value();
  // }
  //
  // catch (...)
  // {
  //   std::cerr << "[fatal] An unexpected error occurred. Report the following output to the developer.\n"
  //             << "\n"
  //             << "\tdeveloper's email: httperror@404-notfound.jp\n"
  //             << "\terrno: " << errno << " - " << std::strerror(errno) << std::endl;
  //
  //   std::exit(errno);
  // }

  meevax::visual_stream vstream {"", 320, 180};

  {
    using namespace meevax;

    vstream << map_raised;

    vstream["main"] << map_raised << flush;

    auto hoge {0};
    std::cin >> hoge; // for stop program while input something
  }

  return 0;
}
