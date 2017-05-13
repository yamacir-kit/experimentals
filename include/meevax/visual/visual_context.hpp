#ifndef INCLUDED_MEEVAX_VISUAL_VISUAL_CONTEXT_HPP_
#define INCLUDED_MEEVAX_VISUAL_VISUAL_CONTEXT_HPP_


#include <memory>
#include <string>

#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>


namespace meevax {


class visual_context
{
  std::unique_ptr<cairo_t, decltype(&cairo_destroy)> cairo_;

  XWindowAttributes attr_;

public:
  visual_context(cairo_surface_t* surface)
    : cairo_ {cairo_create(surface), cairo_destroy}
  {}

  visual_context& operator<<(visual_context& (*f)(visual_context&))
  {
    return (*f)(*this);
  }

  template <typename F>
  visual_context& operator<<(F&& f)
  {
    return f(*this);
  }

public:
  operator cairo_t*() const noexcept
  {
    return cairo_.get();
  }

  operator cairo_surface_t*() const
  {
    return cairo_get_target(cairo_.get());
  }

  operator Display*() const
  {
    return cairo_xlib_surface_get_display(cairo_get_target(cairo_.get()));
  }

  operator Screen*() const
  {
    return cairo_xlib_surface_get_screen(cairo_get_target(cairo_.get()));
  }

  operator Window() const
  {
    return cairo_xlib_surface_get_drawable(cairo_get_target(cairo_.get()));
  }

  operator Visual*() const
  {
    return cairo_xlib_surface_get_visual(cairo_get_target(cairo_.get()));
  }

private:
  void get_window_attributes() noexcept
  {
    XGetWindowAttributes(*this, *this, &attr_);
  }

public:
  auto x() noexcept
  {
    get_window_attributes();
    return attr_.x;
  }

  auto y() noexcept
  {
    get_window_attributes();
    return attr_.y;
  }

  auto width() noexcept
  {
    get_window_attributes();
    return attr_.width;
  }

  auto height() noexcept
  {
    get_window_attributes();
    return attr_.height;
  }

public:
  void move_absolute(int x, int y) noexcept
  {
    XMoveWindow(*this, *this, x, y);
  }

  void move_relative(int x, int y) noexcept
  {
    get_window_attributes();
    XMoveWindow(*this, *this, x + attr_.x, y + attr_.y);
  }

  void resize(std::size_t width, std::size_t height) noexcept
  {
    XResizeWindow(*this, *this, width, height);
  }

  void move_resize(int x, int y, std::size_t width, std::size_t height) noexcept
  {
    XMoveResizeWindow(*this, *this, x, y, width, height);
  }

  void select_input(long event_mask) noexcept
  {
    XSelectInput(*this, *this, event_mask);
  }
};


} // namespace meevax


#endif
