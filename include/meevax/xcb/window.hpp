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
  const std::shared_ptr<xcb_connection_t> connection_;

public:
  const xcb_window_t id;

  explicit window(const std::shared_ptr<xcb_connection_t>& connection, xcb_window_t parent_id)
    : connection_ {connection},
      id {xcb_generate_id(connection_.get())}
  {
    meevax::xcb::accessor<xcb_setup_t, xcb_screen_t> screen {
      xcb_get_setup(connection_.get())
    };

    xcb_create_window(
      connection_.get(),
      XCB_COPY_FROM_PARENT, // depth
      (*this).id, parent_id,
      0, 0,
      std::max<std::uint16_t>(1, 0), std::max<std::uint16_t>(1, 0),
      0,
      XCB_WINDOW_CLASS_INPUT_OUTPUT,
      screen.begin()->root_visual,
      0, nullptr
    );
  }

  const auto& connection() const noexcept
  {
    return connection_;
  }
};


} // namespace meevax::xcb


#endif

