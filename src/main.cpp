#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <system_error>
#include <thread>

#include <meevax/core/generic_editor.hpp>

#include <meevax/vstream/basic_vstream.hpp>
#include <meevax/vstream/vstream_manipulator.hpp>

#include <meevax/version.hpp>


int main(int argc, char** argv)
{
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

  vstream << meevax::raise
          << meevax::color<std::uint8_t>(0x1C, 0x1C, 0x1C)
          << meevax::paint;

  auto show_title = [&]()
  {
    using namespace meevax;

    vstream["title"] << meevax::raise
                     << meevax::color<std::uint8_t>(0x1C, 0x1C, 0x1C)
                     << meevax::paint;

    vstream["title"]((1280-640)/2, (720-200)/4) << resize(640, 200);

    vstream["title"] << color<std::uint8_t>(0xD0, 0xD0, 0xD0)
                     << face("Bitstream Charter") << size(90) << cursorhome
                     << "Meevax System"
                     << face("Noto Sans CJK JP") << size(25) << cr << lf << size(18)
                     << "ぼくのかんがえたさいきょうのえでぃた"
                     << face("Sans") << size(35) << cr << lf
                     << "Version " << project_version.data() << " Alpha "
                     << size(25) << "(" << build_type.data() << " Build)" << endl;
  };

  vstream["debug"]((1280-320)/2, (720-50)*3/4)
    << meevax::resize(320, 50)
    << meevax::color<std::uint8_t>(0x1C, 0x1C, 0x1C)
    << meevax::paint
    << meevax::raise;

  while (true)
  {
    auto event {vstream.next_event()};
    switch (event.type)
    {
    case Expose:
      vstream << meevax::resize(0, 0)
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
