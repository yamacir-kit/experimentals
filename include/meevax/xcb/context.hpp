#ifndef INCLUDED_MEEVAX_XCB_CONTEXT_HPP_
#define INCLUDED_MEEVAX_XCB_CONTEXT_HPP_


#include <memory>

#include <xcb/xcb.h>

#include <meevax/xcb/accessor.hpp>
#include <meevax/xcb/iterator.hpp>


namespace meevax::xcb {


class context
{
  const std::shared_ptr<xcb_connection_t> connection_;
  const                 xcb_window_t      window_id_;

public:
  explicit context(const std::shared_ptr<xcb_connection_t>& connection, const xcb_window_t& parent_window_id)
    : connection_ {connection},
      window_id_ {xcb_generate_id(connection_.get())}
  {
    meevax::xcb::accessor<xcb_setup_t, xcb_screen_t> screen_access {
      xcb_get_setup(connection_.get())
    };

    xcb_create_window(
      connection_.get(),
      XCB_COPY_FROM_PARENT,
      window_id_, parent_window_id,
      0, 0,
      1, 1,
      1,
      XCB_WINDOW_CLASS_INPUT_OUTPUT,
      screen_access.begin()->root_visual,
      0, nullptr
    );
  }

  const auto& connection() const noexcept
  {
    return connection_;
  }

  const auto& window_id() const noexcept
  {
    return window_id_;
  }
};


} // namespace meevax::xcb


#endif

