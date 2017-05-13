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

public:
  auto width() const
  {
    return cairo_xlib_surface_get_width(cairo_get_target(cairo_.get()));
  }

  auto height() const
  {
    return cairo_xlib_surface_get_height(cairo_get_target(cairo_.get()));
  }

public:
  void move_absolute(int x, int y) noexcept
  {
    XMoveWindow(*this, *this, x, y);
  }
};


} // namespace meevax


#endif
