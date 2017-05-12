#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <system_error>
#include <thread>

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
  vstream << meevax::map_raised;

  std::string hello {"Hello, X Window System with cairo vector graphics library"};

  while (true)
  {
    vstream["subwin"] << meevax::map_raised
                      << meevax::color(0, 0, 0) << meevax::paint;

    vstream["subwin"] << meevax::font_face("Ricty Diminished");

    cairo_set_font_size(vstream["subwin"], 20);
    cairo_set_source_rgba(vstream["subwin"], 1.0, 1.0, 1.0, 1.0);

    cairo_move_to(vstream["subwin"], 10, 25);
    cairo_show_text(vstream["subwin"], hello.c_str());

    vstream["subwin"] << meevax::flush;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    if (!hello.empty())
    {
      hello.erase(hello.begin());
    }

    else break;
  }

  return 0;
}
