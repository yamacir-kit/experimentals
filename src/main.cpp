#include <chrono>
#include <cstdlib>
#include <iostream>
#include <system_error>
#include <thread>

#include <meevax/graph/labeled_tree.hpp>

#include <meevax/vstream/basic_vstream.hpp>
#include <meevax/vstream/vstream_manipulator.hpp>
#include <meevax/vstream/vstream_operator.hpp>

#include <meevax/xcb/accessor.hpp>
#include <meevax/xcb/iterator.hpp>
#include <meevax/xcb/window.hpp>

#include <meevax/cairo/surface.hpp>

#include <meevax/version.hpp>


int main(int argc, char** argv) try
{
  std::cout << "[debug] boost version: " << boost_version.data() << "\n";
  std::cout << "[debug] cairo version: " << cairo_version_string() << "\n\n";

  {
    const std::shared_ptr<xcb_connection_t> connection {
      xcb_connect(nullptr, nullptr), xcb_disconnect
    };

    meevax::basic_xcb_vstream<char> vstream {connection};

    vstream.map();
    vstream.resize(640, 480);

    while (true)
    {
      std::unique_ptr<cairo_t, decltype(&cairo_destroy)> cairo {
        cairo_create(vstream.get()), cairo_destroy
      };

      cairo_set_source_rgba(cairo.get(), 1.0, 0.0, 0.0, 0.5);
      cairo_paint(cairo.get());

      cairo_surface_flush(vstream.get());
      xcb_flush(vstream.connection.get());

      std::this_thread::sleep_for(std::chrono::milliseconds {100});
    }

    return 0;
  }

  std::vector<std::string> argv_ {argv, argv + argc};

  // const std::shared_ptr<xcb_connection_t> connection {
  //   xcb_connect(nullptr, nullptr), xcb_disconnect
  // };
  //
  // meevax::graph::labeled_tree<
  //   std::string, meevax::basic_xcb_vstream<char>
  // > vstream {connection};

  std::unique_ptr<Display, decltype(&XCloseDisplay)> display {XOpenDisplay(""), XCloseDisplay};
  XSynchronize(display.get(), true);

  meevax::graph::labeled_tree<std::string, meevax::basic_xlib_vstream<char>> vstream {display.get()};

  vstream.resize(1280, 720).raise();
  vstream["title"].move(320, 130).resize(640, 150).raise();
  vstream["debug"].move(600, 500).resize(320,  50).raise();
  vstream["hello"].move( 50, 400).resize(500, 300).raise();
  vstream["count"].resize(300, 60).raise();

  auto show_title = [&]()
  {
    vstream["title"]
      << meevax::face("Bitstream Charter")
      << meevax::size(90) << meevax::cursorhome
      << "\e[48;2;28;28;28m\e[38;2;208;208;208mMeevax System"
      << meevax::face("Sans")
      << meevax::size(35) << "\n" << meevax::size(25)
      << "Version " << project_version.data() << " Alpha "
      << meevax::size(16)
      << "(" << cmake_build_type.data() << " Build)";
  };

  auto show_hello_world = [&]()
  {
    vstream["hello"]
      << meevax::face("Ricty Diminished")
      << meevax::size(12.0 + 1.5 * 10)
      << meevax::cursorhome
      << "\e[48;2;28;28;28m"
      << "\e[38;2;175;135;255m#include \e[38;2;255;215;95m<iostream>\n"
      << "\n"
      << "\e[38;2;135;215;95mint \e[38;2;208;208;208mmain("
      << "\e[38;2;135;215;95mint \e[38;2;208;208;208margc, "
      << "\e[38;2;135;215;95mchar\e[38;2;208;208;208m** argv)\n"
      << "{\n"
      << "  std::cout << \e[38;2;255;215;95m\"hello, world!\e[38;2;255;95;0m\\n"
      << "\e[38;2;255;215;95m\"\e[38;2;208;208;208m;\n"
      << "\n"
      << "  \e[38;2;255;0;135mreturn \e[38;2;00;215;215m0\e[38;2;208;208;208m;\n"
      << "}";
  };

  while (true)
  {
    auto event {vstream.event()};

    switch (event.type)
    {
    case Expose:
      if (!event.xexpose.count)
      {
        vstream.resize(0, 0) << "\e[48;2;28;28;28m";

        vstream["count"]
          << meevax::face("Ricty Diminished") << meevax::size(12.0 + 1.5 * 10)
          << meevax::cursorhome
          << "\e[48;2;28;28;28m\e[38;2;208;208;208mserial: " << std::to_string(event.xexpose.serial);

        show_title();
        show_hello_world();

        vstream["debug"]
          << meevax::face("Ricty Diminished") << meevax::size(12.0 + 1.5 * 10)
          << meevax::cursorhome
          << "\e[48;2;28;28;28m\e[38;2;208;208;208m[debug] press any key";

        vstream["hoge"].move(0, 50).resize(300, 300).raise() << "\e[48;2;255;0;0m";
        vstream["hoge"]["fuga"].move(0, 50).resize(200, 200).raise() << "\e[48;2;0;255;0m";
        vstream["hoge"]["fuga"]["piyo"].move(0, 50).resize(100, 100).raise() << "\e[48;2;0;0;255m";
      }

      break;

    case KeyPress:
      vstream["debug"]
        << meevax::face("Ricty Diminished") << meevax::size(12.0 + 1.5 * 10)
        << meevax::cursorhome
        << "\e[48;2;28;28;28m\e[38;2;208;208;208m[debug] " << XKeysymToString(XLookupKeysym(&event.xkey, 0));

      break;
    }
  }

  return 0;
}

catch (const std::exception& error)
{
  std::cerr << "[error] standard exception occurred - " << error.what() << std::endl;
  std::exit(EXIT_FAILURE);
}

catch (const std::system_error& error)
{
  std::cerr << "[error] system error occurred\n"
            << "        code: " << error.code().value() << " - " << error.code().message() << std::endl;
  std::exit(error.code().value());
}

catch (...)
{
  std::cerr << "[fatal] unexpected error occurred. report this to the developer.\n"
            << "        developer's email: httperror@404-notfound.jp\n";
  std::exit(errno);
}

