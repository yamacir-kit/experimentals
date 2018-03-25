#ifndef INCLUDED_MEEVAX_XCB_ASCII_KEYBOARD_HPP
#define INCLUDED_MEEVAX_XCB_ASCII_KEYBOARD_HPP


#include <memory>
#include <string>
#include <unordered_map>

#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>


namespace meevax::xcb {


// TODO 見た目には解りやすいコードだけど絶対遅いから綺麗に直すこと
//      std::array<
//        std::array<Char, SIZE_OF_ASCII_PRINTABLE_CHARACTERS>,
//        XCB_MOD_MASK_SHIFT | XCB_MOD_MASK_LOCK | XCB_MOD_MASK_CONTROL
//      > table
//      でも作って
//      array[reinterpret_cast<xcb_key_press_event_t*>(generic_event.get())->state
//            & (XCB_MOD_MASK_SHIFT | XCB_MOD_MASK_LOCK | XCB_MOD_MASK_CONTROL)][keysym - 0x20]
//      とでもすれば使いやすいかも


template <typename Char>
class ascii_keyboard
{
  std::unique_ptr<xcb_key_symbols_t, decltype(&xcb_key_symbols_free)> symbols_;

  // XXX デバッグ用とか備忘録的な意味も兼ねてmapped_type::secondに説明を含めたけど、
  //     実用的な意味ではmapped_type::firstだけあればいいんじゃねえかな
  static const std::unordered_map<
    std::basic_string<Char>, std::pair<Char, std::basic_string<Char>>
  > control_characters;

public:
  Char code;

  explicit ascii_keyboard(const std::shared_ptr<xcb_connection_t>& connection)
    : symbols_ {xcb_key_symbols_alloc(connection.get()), xcb_key_symbols_free}
  {}

  // XXX UGLY CODE
  auto press(const std::shared_ptr<xcb_generic_event_t>& generic_event) noexcept
    -> bool // is able to output as ASCII code
  {
    auto keysym {xcb_key_press_lookup_keysym(
      symbols_.get(),
      reinterpret_cast<xcb_key_press_event_t*>(generic_event.get()),
      reinterpret_cast<xcb_key_press_event_t*>(generic_event.get())->state & ~XCB_MOD_MASK_CONTROL
    )};

    if (xcb_is_modifier_key(keysym))
    {
      return false;
    }

    switch (reinterpret_cast<xcb_key_press_event_t*>(generic_event.get())->state)
    {
    case XCB_MOD_MASK_CONTROL:
    case XCB_MOD_MASK_CONTROL | XCB_MOD_MASK_SHIFT:
      {
        const std::string caret {'^', static_cast<Char>(std::toupper(keysym))};
        if (control_characters.find(caret) != std::end(control_characters))
        {
          code = control_characters.at(caret).first;
          return true;
        }
      }
      break;

    default:
      if (0x1F < keysym && keysym < 0x7F)
      {
        code = static_cast<Char>(keysym);
        return true;
      }
      break;
    }

    return false;
  }
};



template <typename Char>
const std::unordered_map<
  std::basic_string<Char>, std::pair<Char, std::basic_string<Char>>
> meevax::xcb::ascii_keyboard<Char>::control_characters {
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
  {"^2", {0x00, "NUL: Null"}},
  {"^3", {0x1B, "ESC: Escape"}},
  {"^4", {0x1C, "FS: File Separator"}},
  {"^5", {0x1D, "GS: Group Separator"}},
  {"^6", {0x1E, "RS: Record Separator"}},
  {"^7", {0x1F, "US: Unit Separator"}},
  {"^8", {0x7F, "DEL: Delete"}},
};


} // namespace meevax::xcb


#endif

