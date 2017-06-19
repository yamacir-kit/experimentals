#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <system_error>
#include <thread>

#include <meevax/core/generic_editor.hpp>

#include <meevax/vstream/basic_vstream.hpp>
#include <meevax/vstream/vstream_manipulator.hpp>
#include <meevax/vstream/vstream_operator.hpp>


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

  vstream["master"] << meevax::raise;

  auto resize = [&](auto&& width = 0, auto&& height = 0)
  {
    return [&](auto& cairo) -> auto&
    {
      auto surface {cairo_get_target(cairo.get())};
      auto display {cairo_xlib_surface_get_display(surface)};
      auto window {cairo_xlib_surface_get_drawable(surface)};

      XWindowAttributes attr {};
      XGetWindowAttributes(display, window, &attr);

      XResizeWindow(
        display,
        window,
        std::forward<decltype(width)>(width != 0 ? width : attr.width),
        std::forward<decltype(height)>(height != 0 ? height : attr.height)
      );

      cairo_xlib_surface_set_size(surface, attr.width, attr.height);

      return cairo;
    };
  };


  [&]()
  {
    // for (double multiplex {1.0}; multiplex < 80; multiplex += 0.1)
    // {
    //   vstream["master"] << resize(16 * multiplex, 9 * multiplex);
    //   std::this_thread::sleep_for(std::chrono::milliseconds(5));
    // }
    //
    // for (double multiplex {80.0}; 1.0 < multiplex; multiplex -= 0.1)
    // {
    //   vstream["master"] << resize(16 * multiplex, 9 * multiplex);
    //   std::this_thread::sleep_for(std::chrono::milliseconds(5));
    // }

    for (double multiplex {1.0}; multiplex < 80; multiplex += 0.1)
    {
      vstream["master"] << resize(16 * multiplex, 9 * multiplex);
                        // << meevax::color(1, 0, 0) << meevax::paint;
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    // vstream.create("hoge", "master");
    // vstream["hoge"] << meevax::raise << meevax::color(1, 1, 1) << meevax::paint;;

    for (double multiplex {1.0}; multiplex < 80; multiplex += 0.1)
    {
      using namespace meevax;

      vstream["master"] << color(1, 1, 1) << paint;

      vstream["master"] << face("Sans") << color(0, 0, 0)
                        << size(multiplex) << cursorhome<< "Meevax System"
                        << size(multiplex * 0.5) << cr << lf << "Version 0.2.1 Alpha" << endl;

      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    return 0;
  }();

  while (true)
  {
    auto event {vstream.next_event()};
    switch (event.type)
    {
    case Expose:
      // std::cout << "[debug] expose: " << event.xany.serial << std::endl;
      vstream["master"] << resize(0, 0);
      break;

    case KeyPress:
      vstream["master"]
        << meevax::color(0, 0, 0) << meevax::paint
        << meevax::color(1, 1, 1) << meevax::cursorhome
        << "[debug] " << XKeysymToString(XLookupKeysym(&event.xkey, 0)) << meevax::cr;
      break;
    }
  }


  return 0;
}
