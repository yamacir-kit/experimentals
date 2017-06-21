#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <system_error>
#include <thread>

#include <meevax/core/generic_editor.hpp>

#include <meevax/vstream/basic_vstream.hpp>
#include <meevax/vstream/vstream_manipulator.hpp>


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


  meevax::basic_vstream<char> vstream {""};

  vstream << meevax::raise;

  [&]()
  {
    for (double multiplex {1.0}; multiplex < 80; multiplex += 0.1)
    {
      vstream << meevax::resize(16 * multiplex, 1);
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

    for (double multiplex {1.0}; multiplex < 80; multiplex += 0.1)
    {
      vstream << meevax::resize(16 * 80, 9 * multiplex);
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

    return 0;
  }();

  auto show_title = [&]()
  {
    using namespace meevax;

    vstream["title"] << meevax::raise << meevax::color(1, 1, 1) << meevax::paint;

    vstream["title"]((1280-640)/2, (720-160)/4) << resize(640, 160);

    vstream["title"] << face("Sans") << color(0, 0, 0)
                     << size(80) << cursorhome << "Meevax System"
                     << size(40) << cr << lf << "Version 0.2.1 Alpha" << endl;
  };

  vstream["debug"]((1280-320)/2, (720-50)*3/4) << meevax::resize(320, 50) << meevax::raise;

  auto layer_test = [&]()
  {
    vstream["layer1"]
      << meevax::raise
      << meevax::resize(300, 300) << meevax::color(1, 0, 0) << meevax::paint;

    vstream["layer1"]["layer2"]
      << meevax::raise
      << meevax::resize(200, 200) << meevax::color(0, 1, 0) << meevax::paint;

    vstream["layer1"]["layer2"]["layer3"]
      << meevax::raise
      << meevax::resize(100, 100) << meevax::color(0, 0, 1) << meevax::paint;
  };

  while (true)
  {
    auto event {vstream.next_event()};
    switch (event.type)
    {
    case Expose:
      std::cout << "\r[debug] expose: " << event.xexpose.count;
      vstream << meevax::resize(0, 0);
      show_title();
      layer_test();
      break;

    case KeyPress:
      vstream["debug"]
        << meevax::color(1, 1, 1) << meevax::paint
        << meevax::face("Ricty Diminished") << meevax::size(12.0 + 1.5 * 10)
        << meevax::color(0, 0, 0) << meevax::cursorhome
        << "[debug] " << XKeysymToString(XLookupKeysym(&event.xkey, 0)) << meevax::cr;
      break;
    }
  }


  return 0;
}
