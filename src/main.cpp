#include <iostream>
#include <system_error>

#include <meevax/configure/version.hpp>
#include <meevax/graph/dynamic_tree.hpp>
#include <meevax/vstream/basic_vstream.hpp>
#include <meevax/xcb/ascii_keyboard.hpp>


namespace meevax {

class shared_connection
  : public std::shared_ptr<xcb_connection_t>
{
public:
  template <typename... Ts>
  explicit shared_connection(Ts&&... args)
    : std::shared_ptr<xcb_connection_t> {xcb_connect(std::forward<Ts>(args)...), xcb_disconnect}
  {
    if (xcb_connection_has_error(std::shared_ptr<xcb_connection_t>::get()))
    {
      std::cerr << "[error] xcb_connect - failed to connect X server\n";
      std::exit(EXIT_FAILURE);
    }
  }
};

} // namespace meevax


int main(int argc, char** argv) try
{
  std::cout << "[debug] boost version: " << meevax::boost_version.data() << "\n";
  std::cout << "[debug] cairo version: " << cairo_version_string() << "\n\n";

  const meevax::shared_connection connection {nullptr, nullptr};

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

    vstream["input"].map();
    vstream["input"].configure(
      XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
      std::vector<std::uint32_t> {640, 180}.data()
    );
    vstream["input"].configure(
      XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,
      std::vector<std::uint32_t> {0, 0}.data()
    );
    cairo_xcb_surface_set_size(vstream["input"].meevax::cairo::surface::get(), 640, 320);

    vstream["output"].map();
    vstream["output"].configure(
      XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
      std::vector<std::uint32_t> {640, 180}.data()
    );
    vstream["output"].configure(
      XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,
      std::vector<std::uint32_t> {0, 180}.data()
    );
    cairo_xcb_surface_set_size(vstream["output"].meevax::cairo::surface::get(), 640, 320);
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
          std::unique_ptr<cairo_t, decltype(&cairo_destroy)> context {
            cairo_create(surface.meevax::cairo::surface::get()), cairo_destroy
          };

          cairo_set_source_rgba(context.get(), 1.0, 1.0, 1.0, 1.0);
          cairo_paint(context.get());

          return surface;
        };
      }

      break;

    case XCB_KEY_PRESS:
      if (keyboard.press(generic_event))
      {
        vstream["input"] << keyboard.code;
        vstream["output"] << vstream["input"];

        (vstream["input"], std::cout << "\r") << vstream["output"] << std::flush;
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

