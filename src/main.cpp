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
  master.resize(320, 180);

  xcb_flush(connection.get());

  for (std::unique_ptr<xcb_generic_event_t> generic_event {nullptr};
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
      [&]()
      {
        auto deleter = [&](auto pointer) { std::free(pointer); };
        std::unique_ptr<xcb_key_symbols_t, decltype(deleter)> symbols {
          xcb_key_symbols_alloc(connection.get()), deleter
        };

        if (!symbols) { return; }

        auto keysym {xcb_key_press_lookup_keysym(
          symbols.get(), reinterpret_cast<xcb_key_press_event_t*>(generic_event.get()), 0
        )};

        std::cout << "[debug] keysym: " << keysym << "\n";
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

