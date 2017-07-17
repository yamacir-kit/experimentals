#ifndef INCLUDED_MEEVAX_VSTREAM_BASIC_XLIB_VSTREAM_HPP_
#define INCLUDED_MEEVAX_VSTREAM_BASIC_XLIB_VSTREAM_HPP_


#include <string>
#include <memory>
#include <utility>

// #include <X11/Xlib.h>
// #include <cairo/cairo-xlib.h>

#include <meevax/cairo/context.hpp>


namespace meevax {


template <typename C>
class basic_xlib_vstream
  // : public std::unique_ptr<cairo_t, decltype(&cairo_destroy)>
  : public meevax::cairo::xlib::context
{
  XEvent event_;

public:
  explicit basic_xlib_vstream(Display* display)
    // : std::unique_ptr<cairo_t, decltype(&cairo_destroy)> {
    //     create(display, XDefaultRootWindow(display)), cairo_destroy
    //   }
    : meevax::cairo::xlib::context {display, XDefaultRootWindow(display)}
  {}

  explicit basic_xlib_vstream(const meevax::basic_xlib_vstream<C>& vstream)
    // : std::unique_ptr<cairo_t, decltype(&cairo_destroy)> {
    //     create(static_cast<Display*>(vstream), static_cast<Window>(vstream)), cairo_destroy
    //   }
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
    XNextEvent(static_cast<Display*>(*this), &event_);
    return event_;
  }

public: // Cairo Cast Operators
  // explicit operator cairo_t*() const noexcept
  // {
  //   return std::unique_ptr<cairo_t, decltype(&cairo_destroy)>::get();
  // }
  //
  // explicit operator cairo_surface_t*() const noexcept
  // {
  //   return cairo_get_target(static_cast<cairo_t*>(*this));
  // }

private:
//   explicit operator Display*() const noexcept
//   {
//     return cairo_xlib_surface_get_display(static_cast<cairo_surface_t*>(*this));
//   }
//
//   explicit operator Window() const noexcept
//   {
//     return cairo_xlib_surface_get_drawable(static_cast<cairo_surface_t*>(*this));
//   }
//
//   auto create(Display* display, const Window& parent)
//   {
//     static constexpr std::size_t default_window_width  {1280};
//     static constexpr std::size_t default_window_height { 720};
//
// #ifndef NDEBUG
//     static constexpr std::size_t default_border_width  {   1};
// #else
//     static constexpr std::size_t default_border_width  {   0};
// #endif
//
//     auto window {XCreateSimpleWindow(display, parent, 0, 0, default_window_width, default_window_height, default_border_width, XBlackPixel(display, XDefaultScreen(display)), XWhitePixel(display, XDefaultScreen(display)))};
//     auto surface {cairo_xlib_surface_create(display, window, XDefaultVisual(display, XDefaultScreen(display)), default_window_width, default_window_height)};
//
//     XSelectInput(display, window, ExposureMask | KeyPressMask); // TODO functionize
//
//     return cairo_create(surface);
//   }
};


} // namespace meevax


#endif

