#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <system_error>
#include <thread>

#include <meevax/core/generic_editor.hpp>

#include <meevax/vstream/basic_vstream.hpp>
#include <meevax/vstream/graphix_manipulator.hpp>
#include <meevax/vstream/graphix_operator.hpp>

#include <meevax/version.hpp>


int main(int argc, char** argv)
{
  std::cout << "[debug] cairo version: " << cairo_version_string() << std::endl;

  if (false)
  {
    meevax::generic_editor<char> editor {argc, argv};

    try
    {
      while (true)
      {
        editor.write();
        editor.read();
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

  std::unique_ptr<Display, decltype(&XCloseDisplay)> display {XOpenDisplay(""), XCloseDisplay};

  meevax::visual_node<char> vstream {display.get()};

  vstream.raise()
    << meevax::color<std::uint8_t>(0x1C, 0x1C, 0x1C)
    << meevax::paint;

  auto show_title = [&]()
  {
    vstream["title"].raise()
      << meevax::color<std::uint8_t>(0x1C, 0x1C, 0x1C)
      << meevax::paint;

    vstream["title"].move((1280-640)/2, (720-200)/4);
    vstream["title"].resize(640, 200);

    vstream["title"]
      << meevax::color<std::uint8_t>(0xD0, 0xD0, 0xD0)
      << meevax::face("Bitstream Charter")
      << meevax::size(90) << meevax::cursorhome
      << "Meevax System"
      << meevax::face("Sans")
      << meevax::size(35) << meevax::endl << meevax::size(25)
      << "Version " << project_version.data() << " Alpha "
      << meevax::size(16)
      << "(" << build_type.data() << " Build)" << meevax::endl;
  };

  vstream["debug"].move((1280-320)/2, (720-50)*3/4).resize(320, 50).raise()
    << meevax::color<std::uint8_t>(0x1C, 0x1C, 0x1C)
    << meevax::paint;

  while (true)
  {
    auto event {vstream.event()};
    switch (event.type)
    {
    case Expose:
      vstream.resize(0, 0)
        << meevax::color<std::uint8_t>(0x1C, 0x1C, 0x1C)
        << meevax::paint;

      show_title();

      vstream["debug"]
        << meevax::color<std::uint8_t>(0x1C, 0x1C, 0x1C) << meevax::paint
        << meevax::face("Ricty Diminished") << meevax::size(12.0 + 1.5 * 10)
        << meevax::color<std::uint8_t>(0xD0, 0xD0, 0xD0) << meevax::cursorhome
        << "[debug] " << "press any key" << meevax::cr;

      break;

    case KeyPress:
      vstream["debug"]
        << meevax::color<std::uint8_t>(0x1C, 0x1C, 0x1C) << meevax::paint
        << meevax::face("Ricty Diminished") << meevax::size(12.0 + 1.5 * 10)
        << meevax::color<std::uint8_t>(0xD0, 0xD0, 0xD0) << meevax::cursorhome
        << "[debug] " << XKeysymToString(XLookupKeysym(&event.xkey, 0)) << meevax::cr;

      break;
    }
  }


  return 0;
}
