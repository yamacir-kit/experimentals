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

  Window root_window_;
  int x_, y_;
  unsigned int width_, height_, border_width_, depth_;

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
  void update_getometry() noexcept
  {
    XGetGeometry(*this, *this, &root_window_, &x_, &y_, &width_, &height_, &border_width_, &depth_);
  }

public:
  auto x() noexcept
  {
    update_getometry();
    return x_;
  }

  auto y() noexcept
  {
    update_getometry();
    return y_;
  }

  auto width() noexcept
  {
    update_getometry();
    return width_;
  }

  auto height() noexcept
  {
    update_getometry();
    return height_;
  }

public:
  void move_absolute(int x, int y) noexcept
  {
    XMoveWindow(*this, *this, x, y);
  }

  void move_relative(int x, int y) noexcept
  {
    update_getometry();
    XMoveWindow(*this, *this, x + x_, y + y_);
  }

  void resize(std::size_t width, std::size_t height) noexcept
  {
    XResizeWindow(*this, *this, width, height);
  }

  void move_resize(int x, int y, std::size_t width, std::size_t height) noexcept
  {
    XMoveResizeWindow(*this, *this, x, y, width, height);
  }
};


} // namespace meevax


#endif
