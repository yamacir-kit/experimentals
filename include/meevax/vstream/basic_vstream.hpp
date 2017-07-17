#ifndef INCLUDED_MEEVAX_VSTREAM_BASIC_XLIB_VSTREAM_HPP_
#define INCLUDED_MEEVAX_VSTREAM_BASIC_XLIB_VSTREAM_HPP_


#include <string>
#include <memory>
#include <utility>

#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>

#include <meevax/cairo/context.hpp>


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
using basic_vstream = basic_xlib_vstream<C>;


} // namespace meevax


#endif

