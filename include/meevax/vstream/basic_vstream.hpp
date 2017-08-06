#ifndef INCLUDED_MEEVAX_VSTREAM_BASIC_XLIB_VSTREAM_HPP_
#define INCLUDED_MEEVAX_VSTREAM_BASIC_XLIB_VSTREAM_HPP_


#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>

#include <xcb/xcb.h>
#include <cairo/cairo-xcb.h>

#include <meevax/cairo/surface.hpp>


namespace meevax {


template <typename C>
class basic_xlib_vstream
  : public meevax::cairo::xlib::context
{
public:
  explicit basic_xlib_vstream(Display* display)
    : meevax::cairo::xlib::context {display, XDefaultRootWindow(display)}
  {}

  explicit basic_xlib_vstream(const meevax::basic_xlib_vstream<C>& vstream)
    : meevax::cairo::xlib::context {static_cast<Display*>(vstream), static_cast<Window>(vstream)}
  {}

public:
  auto& map() const noexcept
  {
    XMapWindow(static_cast<Display*>(*this), static_cast<Window>(*this));
    return *this;
  }

  auto& unmap() const noexcept
  {
    XUnmapWindow(static_cast<Display*>(*this), static_cast<Window>(*this));
    return *this;
  }

  auto& raise() const noexcept
  {
    XMapRaised(static_cast<Display*>(*this), static_cast<Window>(*this));
    return *this;
  }

  auto& move(std::size_t x, std::size_t y) const noexcept
  {
    XMoveWindow(static_cast<Display*>(*this), static_cast<Window>(*this), x, y);
    return *this;
  }

  auto& resize(std::size_t width, std::size_t height) const noexcept
  {
    static XWindowAttributes a;
    XGetWindowAttributes(static_cast<Display*>(*this), static_cast<Window>(*this), &a);

    XResizeWindow(static_cast<Display*>(*this), static_cast<Window>(*this), width != 0 ? width : a.width, height != 0 ? height : a.height);
    cairo_xlib_surface_set_size(static_cast<cairo_surface_t*>(*this), a.width, a.height);

    return *this;
  }

  auto& event()
  {
    static XEvent event_;
    XNextEvent(static_cast<Display*>(*this), &event_);
    return event_;
  }
};


template <typename C>
class basic_xcb_vstream
  : public meevax::cairo::surface
{
  std::vector<std::basic_string<C>> data_;

public:
  template <typename... Ts>
  explicit basic_xcb_vstream(Ts&&... args)
    : meevax::cairo::surface {std::forward<Ts>(args)...}
  {}

public:
  void map() const noexcept
  {
    xcb_map_window(
      meevax::xcb::window::connection.get(),
      meevax::xcb::window::id
    );
  }

  void resize(std::uint32_t width, std::uint32_t height) const noexcept
  {
    xcb_configure_window(
      (*this).connection.get(), (*this).id,
      XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
      std::vector<std::uint32_t> {width, height}.data()
    );

    cairo_xcb_surface_set_size((*this).get(), width, height);
  }
};


template <typename C>
using basic_vstream = basic_xlib_vstream<C>;


} // namespace meevax


#endif

