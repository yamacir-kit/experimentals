#ifndef INCLUDED_MEEVAX_VSTREAM_GRAPHIX_IMPL_HPP_
#define INCLUDED_MEEVAX_VSTREAM_GRAPHIX_IMPL_HPP_


#include <string>
#include <memory>
#include <utility>

#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>


namespace meevax {


class graphix_impl
  // : std::unique_ptr<cairo_t, decltype(&cairo_destroy)>
{
  std::unique_ptr<cairo_t, decltype(&cairo_destroy)> cairo_;
  XEvent event_;

public:
  explicit graphix_impl(Display* display)
    : cairo_ {create(display, XDefaultRootWindow(display)), cairo_destroy}
    // : std::unique_ptr<cairo_t, decltype(&cairo_destroy)> {create(display, XDefaultRootWindow(display)), cairo_destroy}
  {}

  explicit graphix_impl(const meevax::graphix_impl& impl)
    : cairo_ {create(static_cast<Display*>(impl), static_cast<Window>(impl)), cairo_destroy}
    // : std::unique_ptr<cairo_t, decltype(&cairo_destroy)> {create(static_cast<Display*>(impl), static_cast<Window>(impl)), cairo_destroy}
  {}

  auto& next_event() // XXX UGLY CODE
  {
    XNextEvent(static_cast<Display*>(*this), &event_);
    return event_;
  }

public:
  auto& operator()(std::size_t&& x, std::size_t&& y) const
  {
    XMoveWindow(static_cast<Display*>(*this), static_cast<Window>(*this), std::forward<decltype(x)>(x), std::forward<decltype(y)>(y));
    return *this;
  }

public:
  explicit operator cairo_t*() const noexcept
  {
    return cairo_.get();
  }

  explicit operator cairo_surface_t*() const noexcept
  {
    return cairo_get_target(static_cast<cairo_t*>(*this));
  }

  explicit operator Display*() const noexcept
  {
    return cairo_xlib_surface_get_display(static_cast<cairo_surface_t*>(*this));
  }

  explicit operator Window() const noexcept
  {
    return cairo_xlib_surface_get_drawable(static_cast<cairo_surface_t*>(*this));
  }

protected:
  cairo_t* create(Display* display, const Window& parent)
  {
    static constexpr std::size_t default_window_width  {1280};
    static constexpr std::size_t default_window_height { 720};

#ifndef NDEBUG
    static constexpr std::size_t default_border_width  {   1};
#else
    static constexpr std::size_t default_border_width  {   0};
#endif

    auto window {XCreateSimpleWindow(display, parent, 0, 0, default_window_width, default_window_height, default_border_width, XBlackPixel(display, XDefaultScreen(display)), XWhitePixel(display, XDefaultScreen(display)))};
    auto surface {cairo_xlib_surface_create(display, window, XDefaultVisual(display, XDefaultScreen(display)), default_window_width, default_window_height)};

    XSelectInput(display, window, ExposureMask | KeyPressMask); // TODO functionize

    return cairo_create(surface);
  }
};


} // namespace meevax


template <typename F>
auto& operator<<(const meevax::graphix_impl& impl, F&& f)
{
  return f(impl);
}


template <typename C>
auto& operator<<(const meevax::graphix_impl& impl, C* c_str)
{
  cairo_show_text(static_cast<cairo_t*>(impl), c_str);
  return impl;
};


template <typename C>
auto& operator<<(const meevax::graphix_impl& impl, const C* c_str)
{
  cairo_show_text(static_cast<cairo_t*>(impl), c_str);
  return impl;
};


template <typename C>
auto& operator<<(const meevax::graphix_impl& impl, const std::basic_string<C>& text)
{
  cairo_show_text(static_cast<cairo_t*>(impl), text.c_str());
  return impl;
};


#endif

