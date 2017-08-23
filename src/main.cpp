#include <iostream>
#include <system_error>

#include <xcb/xcb_keysyms.h>

#include <meevax/configure/version.hpp>
#include <meevax/graph/labeled_tree.hpp>
#include <meevax/vstream/basic_vstream.hpp>


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

  meevax::graph::labeled_tree<
    std::string, meevax::basic_vstream<char>
  > master {connection};

  master.change_attributes(
    XCB_CW_EVENT_MASK,
    std::vector<std::uint32_t> {XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS}.data()
  );

  master.map();
  master.resize(1280 / 2, 720 / 2);

  xcb_flush(connection.get());

  for (std::shared_ptr<xcb_generic_event_t> generic_event {nullptr};
       generic_event.reset(xcb_wait_for_event(connection.get())), generic_event;)
  {
    switch (generic_event->response_type & ~0x80)
    {
    case XCB_EXPOSE:
      std::cout << "[debug] expose\n";

      master << [&](auto& surface) -> auto& {
        std::unique_ptr<cairo_t, decltype(&cairo_destroy)> cairo {
          cairo_create(surface.get()), cairo_destroy
        };

        cairo_set_source_rgba(cairo.get(), 1.0, 0.0, 0.0, 1.0);
        cairo_paint(cairo.get());

        return surface;
      };

      break;

    case XCB_KEY_PRESS:
      [&]() -> void
      {
        auto keysym {xcb_key_press_lookup_keysym(
          std::unique_ptr<xcb_key_symbols_t, decltype(&xcb_key_symbols_free)> {
            xcb_key_symbols_alloc(connection.get()), xcb_key_symbols_free
          }.get(),
          reinterpret_cast<xcb_key_press_event_t*>(generic_event.get()),
          reinterpret_cast<xcb_key_press_event_t*>(generic_event.get())->state & ~XCB_MOD_MASK_CONTROL
        )};

        switch (auto modmask {reinterpret_cast<xcb_key_press_event_t*>(generic_event.get())->state})
        {
        case XCB_MOD_MASK_CONTROL:
        case XCB_MOD_MASK_CONTROL | XCB_MOD_MASK_SHIFT:
          std::cout << "^" << static_cast<char>(keysym) << std::flush;
          break;

        default:
          if (31 < keysym && keysym < 127)
          {
            std::cout << static_cast<char>(keysym) << std::flush;
          }
          break;
        }
      }();

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

