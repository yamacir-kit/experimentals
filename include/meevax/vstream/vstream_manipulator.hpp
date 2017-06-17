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


auto color = [](double r, double g, double b, double a = 1.0)
{
  return [&](auto& cairo) -> auto&
         {
           cairo_set_source_rgba(cairo.get(), r, g, b, a);
           return cairo;
         };
};


auto face = [](const std::string&  font_family,
               cairo_font_slant_t  font_slant  = CAIRO_FONT_SLANT_NORMAL,
               cairo_font_weight_t font_weight = CAIRO_FONT_WEIGHT_NORMAL) // TODO enum classfy
{
  return [&](auto& cairo) -> auto&
         {
           cairo_select_font_face(cairo.get(), font_family.c_str(), font_slant, font_weight);
           return cairo;
         };
};


auto size = [](auto size)
{
  return [&](auto& cairo) -> auto&
         {
           cairo_set_font_size(cairo.get(), std::forward<decltype(size)>(size));
           return cairo;
         };
};



} // namespace meevax


#endif
