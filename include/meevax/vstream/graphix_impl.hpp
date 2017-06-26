#ifndef INCLUDED_MEEVAX_VSTREAM_GRAPHIX_IMPL_HPP_
#define INCLUDED_MEEVAX_VSTREAM_GRAPHIX_IMPL_HPP_


#include <string>
#include <memory>
#include <utility>

#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>


namespace meevax {


class graphix_impl
  : public std::unique_ptr<cairo_t, decltype(&cairo_destroy)>
{
  XEvent event_;

public:
  explicit graphix_impl(Display* display)
    : std::unique_ptr<cairo_t, decltype(&cairo_destroy)> {
        create(display, XDefaultRootWindow(display)), cairo_destroy
      }
  {}

  explicit graphix_impl(const meevax::graphix_impl& impl)
    : std::unique_ptr<cairo_t, decltype(&cairo_destroy)> {
        create(static_cast<Display*>(impl), static_cast<Window>(impl)), cairo_destroy
      }
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
    static XWindowAttributes attributes {};
    XGetWindowAttributes(static_cast<Display*>(*this), static_cast<Window>(*this), &attributes);

    XResizeWindow(static_cast<Display*>(*this), static_cast<Window>(*this), width != 0 ? width : attributes.width, height != 0 ? height : attributes.height);
    cairo_xlib_surface_set_size(static_cast<cairo_surface_t*>(*this), attributes.width, attributes.height);

    return *this;
  }

  auto& event()
  {
    XNextEvent(static_cast<Display*>(*this), &event_);
    return event_;
  }

public: // Cairo Cast Operators
  explicit operator cairo_t*() const noexcept
  {
    return std::unique_ptr<cairo_t, decltype(&cairo_destroy)>::get();
  }

  explicit operator cairo_surface_t*() const noexcept
  {
    return cairo_get_target(static_cast<cairo_t*>(*this));
  }

private:
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


template <typename F>
auto& operator<<(const meevax::graphix_impl& impl, F&& f)
{
  return f(impl);
}


} // namespace meevax


#endif

