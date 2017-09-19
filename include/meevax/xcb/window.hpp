#ifndef INCLUDED_MEEVAX_XCB_WINDOW_HPP_
#define INCLUDED_MEEVAX_XCB_WINDOW_HPP_


#include <algorithm>
#include <memory>

#include <xcb/xcb.h>

#include <meevax/xcb/accessor.hpp>
#include <meevax/xcb/iterator.hpp>


namespace meevax::xcb {


class window
{
public:
  const std::shared_ptr<xcb_connection_t> connection;
  const xcb_window_t id;

  explicit window(const std::shared_ptr<xcb_connection_t>& connection_, xcb_window_t parent_id)
    : connection {connection_},
      id {xcb_generate_id(connection.get())}
  {
    xcb::accessor<xcb_setup_t, xcb_screen_t> screen {
      xcb_get_setup(connection.get())
    };

    xcb_create_window(
      connection.get(),
      XCB_COPY_FROM_PARENT, // depth
      (*this).id, parent_id,
      0, 0,
      std::max<std::uint16_t>(1, 0), std::max<std::uint16_t>(1, 0),
      1, // DEBUG: border line width
      XCB_WINDOW_CLASS_INPUT_OUTPUT,
      screen.begin()->root_visual,
      0, nullptr
    );
  }

  ~window()
  {
    xcb_destroy_window(connection.get(), id);
  }
};


} // namespace meevax::xcb


#endif

