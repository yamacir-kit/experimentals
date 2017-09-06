#include <iostream>
#include <system_error>

#include <meevax/configure/version.hpp>
#include <meevax/graph/dynamic_tree.hpp>
#include <meevax/vstream/basic_vstream.hpp>
#include <meevax/xcb/ascii_keyboard.hpp>


int main(int argc, char** argv) try
{
  std::cout << "[debug] boost version: " << meevax::boost_version.data() << "\n";
  std::cout << "[debug] cairo version: " << cairo_version_string() << "\n\n";

  const std::shared_ptr<xcb_connection_t> connection {
    xcb_connect(nullptr, nullptr), xcb_disconnect
  };

  if (xcb_connection_has_error(connection.get()))
  {
    std::cerr << "[error] xcb_connect - failed to connect X server\n";
    std::exit(EXIT_FAILURE);
  }

#ifndef NDEBUG
  meevax::basic_vstream<char> debug {connection};
  debug.map();
  debug.configure(
    XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
    std::vector<std::uint32_t> {640, 360}.data()
  );
  cairo_xcb_surface_set_size(debug.meevax::cairo::surface::get(), 640, 320);
#endif

  meevax::graph::dynamic_tree<std::string, meevax::basic_vstream<char>> vstream {connection};
  meevax::xcb::ascii_keyboard<char> keyboard {connection};

  {
    vstream.map();
    vstream.change_attributes(
      XCB_CW_EVENT_MASK,
      std::vector<std::uint32_t> {XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS}.data()
    );
    vstream.configure(
      XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
      std::vector<std::uint32_t> {640, 360}.data()
    );
    cairo_xcb_surface_set_size(vstream.meevax::cairo::surface::get(), 640, 320);

    vstream["raw_input"].map();
    vstream["raw_input"].configure(
      XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
      std::vector<std::uint32_t> {640, 180}.data()
    );
    vstream["raw_input"].configure(
      XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,
      std::vector<std::uint32_t> {0, 0}.data()
    );
    cairo_xcb_surface_set_size(vstream["raw_input"].meevax::cairo::surface::get(), 640, 320);

    vstream["parsed"].map();
    vstream["parsed"].configure(
      XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
      std::vector<std::uint32_t> {640, 180}.data()
    );
    vstream["parsed"].configure(
      XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,
      std::vector<std::uint32_t> {0, 180}.data()
    );
    cairo_xcb_surface_set_size(vstream["parsed"].meevax::cairo::surface::get(), 640, 320);
  }

  xcb_flush(connection.get());

  for (std::shared_ptr<xcb_generic_event_t> generic_event {nullptr};
       generic_event.reset(xcb_wait_for_event(connection.get())), generic_event;
       xcb_flush(connection.get()))
  {
    switch (generic_event->response_type & ~0x80)
    {
    case XCB_EXPOSE:
      if (!reinterpret_cast<xcb_expose_event_t*>(generic_event.get())->count)
      {
        vstream << [&](auto& surface) -> auto&
        {
          std::unique_ptr<cairo_t, decltype(&cairo_destroy)> cairo {
            cairo_create(surface.meevax::cairo::surface::get()), cairo_destroy
          };

          cairo_set_source_rgba(cairo.get(), 1.0, 1.0, 1.0, 1.0);
          cairo_paint(cairo.get());

          return surface;
        };
      }

      break;

    case XCB_KEY_PRESS:
      if (keyboard.press(generic_event))
      {
        vstream["raw_input"] << keyboard.code << std::flush;
        vstream["raw_input"].output();

        vstream["parsed"] << keyboard.code << std::flush;
        vstream["parsed"].output();

#ifndef NDEBUG
        debug.clear();
        if (std::isgraph(keyboard.code))
        {
          debug << "keyboard input: " << keyboard.code << "\n";
        }
        else
        {
          debug << "keyboard input: 0x" << std::hex << static_cast<int>(keyboard.code) << "\n";
        }
        debug.output();
#endif
      }
      break;

    default:
      break;
    }
  }

  return 0;
}

catch (const std::system_error& error)
{
  const auto error_code {error.code().value()};
  std::cerr << "[error] code: " << error_code << " - " << error.code().message() << "\n";
  std::exit(error_code);
}

catch (const std::exception& error)
{
  std::cerr << "[error] " << error.what() << std::endl;
  std::exit(EXIT_FAILURE);
}

