#ifndef INCLUDED_MEEVAX_VSTREAM_VSTREAM_MANIPULATOR_HPP_
#define INCLUDED_MEEVAX_VSTREAM_VSTREAM_MANIPULATOR_HPP_


#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>


template <typename F>
auto& operator<<(const std::unique_ptr<cairo_t, decltype(&cairo_destroy)>& p, F&& f)
{
  return f(p);
}


namespace meevax {


auto map_raised = [](auto& p) -> auto& {
  XMapRaised(
    cairo_xlib_surface_get_display(cairo_get_target(p.get())),
    cairo_xlib_surface_get_drawable(cairo_get_target(p.get()))
  );
  return p;
};


} // namespace meevax


#endif
