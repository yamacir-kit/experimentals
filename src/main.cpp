#include <cassert>
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
        std::unordered_map<std::string, std::pair<char, std::string>> ascii_control_characters {
          {"^@", {0x00, "NUL: Null"}},
          {"^A", {0x01, "SOH: Start of Heading"}},
          {"^B", {0x02, "STX: Start of Text"}},
          {"^C", {0x03, "ETX: End of Text"}},
          {"^D", {0x04, "EOT: End of Transmission"}},
          {"^E", {0x05, "ENQ: Enquiry"}},
          {"^F", {0x06, "ACK: Acknowledgement"}},
          {"^G", {0x07, "BEL: Bell"}},
          {"^H", {0x08, "BS: Backspace"}},
          {"^I", {0x09, "HT: Hrizontal Tab"}},
          {"^J", {0x0A, "LF: Line Feed"}},
          {"^K", {0x0B, "VT: Vertical Tab"}},
          {"^L", {0x0C, "FF: Form Feed"}},
          {"^M", {0x0D, "CR: Carriage Return"}},
          {"^N", {0x0E, "SO: Shift Out"}},
          {"^O", {0x0F, "SI: Shift In"}},
          {"^P", {0x10, "DLE: Data Link Escape"}},
          {"^Q", {0x11, "DC1: Device Control 1"}},
          {"^R", {0x12, "DC2: Device Control 2"}},
          {"^S", {0x13, "DC3: Device Control 3"}},
          {"^T", {0x14, "DC4: Device Control 4"}},
          {"^U", {0x15, "NAK: Negative Acknowledgement"}},
          {"^V", {0x16, "SYN: Synchronous Idle"}},
          {"^W", {0x17, "ETB: End of Transmission Block"}},
          {"^X", {0x18, "CAN: Cancel"}},
          {"^Y", {0x19, "EM: End of Medium"}},
          {"^Z", {0x1A, "SUB: Substitute"}},
          {"^[", {0x1B, "ESC: Escape"}},
          {"^\\",{0x1C, "FS: File Separator"}},
          {"^]", {0x1D, "GS: Group Separator"}},
          {"^^", {0x1E, "RS: Record Separator"}},
          {"^_", {0x1F, "US: Unit Separator"}},
          {"^?", {0x7F, "DEL: Delete"}},
        };

        ascii_control_characters["^2"] = ascii_control_characters["^@"];
        ascii_control_characters["^3"] = ascii_control_characters["^["];
        ascii_control_characters["^4"] = ascii_control_characters["^\\"];
        ascii_control_characters["^5"] = ascii_control_characters["^]"];
        ascii_control_characters["^6"] = ascii_control_characters["^^"];
        ascii_control_characters["^7"] = ascii_control_characters["^_"];
        ascii_control_characters["^8"] = ascii_control_characters["^?"];

        auto keysym {xcb_key_press_lookup_keysym(
          std::unique_ptr<xcb_key_symbols_t, decltype(&xcb_key_symbols_free)> {
            xcb_key_symbols_alloc(connection.get()), xcb_key_symbols_free
          }.get(),
          reinterpret_cast<xcb_key_press_event_t*>(generic_event.get()),
          reinterpret_cast<xcb_key_press_event_t*>(generic_event.get())->state & ~XCB_MOD_MASK_CONTROL
        )};

        if (xcb_is_modifier_key(keysym))
        {
          return;
        }

        switch (auto modmask {reinterpret_cast<xcb_key_press_event_t*>(generic_event.get())->state})
        {
        case XCB_MOD_MASK_CONTROL:
        case XCB_MOD_MASK_CONTROL | XCB_MOD_MASK_SHIFT:
          {
            std::string caret {'^', static_cast<char>(std::toupper(keysym))};
            if (ascii_control_characters.find(caret) != std::end(ascii_control_characters))
            {
              std::cout << "[debug] " << caret << ", ("
                        << ascii_control_characters.at(caret).second << ")" << std::endl;
            }
          }
          break;

        default:
          if (0x1F < keysym && keysym < 0x7F)
          {
            std::cout << "[debug] " << static_cast<char>(keysym) << std::endl;
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

