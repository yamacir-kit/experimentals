#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <system_error>
#include <thread>

#include <meevax/core/generic_editor.hpp>

#include <meevax/visual/visual_stream.hpp>
#include <meevax/visual/visual_manipulator.hpp>

#include <meevax/vstream/basic_vstream.hpp>
#include <meevax/vstream/vstream_manipulator.hpp>


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

  // meevax::visual_stream vstream {"", 160 * 4, 90 * 4};
  // vstream << meevax::map_raised << meevax::flush;
  //
  // std::string hello {"Hello, X Window System with cairo vector graphics library"};
  //
  // while (true)
  // {
  //   vstream["subwin"] << meevax::map_raised;
  //   vstream["subwin"].move_absolute(80, 45);
  //
  //   vstream["subwin"] << meevax::color(1, 1, 1) << meevax::paint;
  //
  //   vstream["subwin"] << meevax::font_face("Ricty Diminished")
  //                     << meevax::font_size(20)
  //                     << meevax::color(0, 0, 0);
  //
  //   vstream["subwin"] << meevax::move_to(10, 25) << meevax::text(hello) << meevax::flush;
  //
  //   // std::this_thread::sleep_for(std::chrono::milliseconds(100));
  //
  //   if (!hello.empty())
  //   {
  //     hello.erase(hello.begin());
  //   }
  //
  //   else break;
  // }
  //
  // // vstream.erase("subwin");
  //
  // vstream["event_test"].resize(320, 180);
  // vstream["event_test"].move_absolute(160, 90);
  // vstream["event_test"].select_inputs(ExposureMask | KeyPressMask);
  //
  // vstream["event_test"] << meevax::map_raised;
  //
  // vstream["event_test"] << meevax::font_face("Ricty Diminished") << meevax::font_size(20)
  //                       << meevax::color(0, 0, 0);
  //
  // vstream.event_process([&](auto event)
  // {
  //   switch (event.type)
  //   {
  //   case Expose:
  //     std::cout << "[debug] expose\n";
  //     break;
  //
  //   case KeyPress:
  //     std::cout << "[debug] " << XKeysymToString(XLookupKeysym(&event.xkey, 0)) << std::endl;
  //     break;
  //   }
  // });

  meevax::basic_vstream<char> vstream {""};

  auto map_raised = [](auto& p) -> auto& {
    XMapRaised(
      cairo_xlib_surface_get_display(cairo_get_target(p.get())),
      cairo_xlib_surface_get_drawable(cairo_get_target(p.get()))
    );
    return p;
  };

  auto color = [](auto& p) -> auto& {
    cairo_set_source_rgba(p.get(), 0, 0, 0, 1);
    return p;
  };

  auto text = [](auto& p) -> auto& {
    cairo_show_text(p.get(), "hogehoge");
    return p;
  };

  vstream["master"] << map_raised << color << text;

  sleep(3);

  return 0;
}
