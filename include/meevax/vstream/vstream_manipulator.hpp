#ifndef INCLUDED_MEEVAX_VSTREAM_VSTREAM_MANIPULATOR_HPP_
#define INCLUDED_MEEVAX_VSTREAM_VSTREAM_MANIPULATOR_HPP_


#include <memory>
#include <string>

#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>


template <typename F>
auto& operator<<(const std::unique_ptr<cairo_t, decltype(&cairo_destroy)>& p, F&& f)
{
  return f(p);
}


namespace meevax {


auto map_raised = [](auto& cairo)
  -> auto&
{
  XMapRaised(
    cairo_xlib_surface_get_display(cairo_get_target(cairo.get())),
    cairo_xlib_surface_get_drawable(cairo_get_target(cairo.get()))
  );
  return cairo;
};


auto flush = [](auto& cairo)
  -> auto&
{
  cairo_surface_flush(cairo_get_target(cairo.get()));
  return cairo;
};


auto color = [](double&& r, double&& g, double&& b, double&& a = 1.0)
{
  return [&](auto& cairo) -> auto&
         {
           cairo_set_source_rgba(cairo.get(), std::forward<decltype(r)>(r), std::forward<decltype(g)>(g), std::forward<decltype(b)>(b), std::forward<decltype(a)>(a));
           return cairo;
         };
};


auto face = [](const std::string&  family,
               cairo_font_slant_t&&  slant  = CAIRO_FONT_SLANT_NORMAL,
               cairo_font_weight_t&& weight = CAIRO_FONT_WEIGHT_NORMAL)
{
  return [&](auto& cairo) -> auto&
         {
           cairo_select_font_face(cairo.get(), family.c_str(), std::forward<decltype(slant)>(slant), std::forward<decltype(weight)>(weight));
           return cairo;
         };
};


auto size = [](auto&& size)
{
  return [&](auto& cairo) -> auto&
         {
           cairo_set_font_size(cairo.get(), std::forward<decltype(size)>(size));
           return cairo;
         };
};


auto move = [](auto&& x, auto&& y)
{
  return [&](auto& cairo) -> auto&
         {
           cairo_move_to(cairo.get(), std::forward<decltype(x)>(x), std::forward<decltype(y)>(y));
           return cairo;
         };
};


} // namespace meevax


#endif
