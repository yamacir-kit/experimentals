#ifndef INCLUDED_MEEVAX_VSTREAM_VSTREAM_MANIPULATOR_HPP_
#define INCLUDED_MEEVAX_VSTREAM_VSTREAM_MANIPULATOR_HPP_


#include <memory>
#include <string>

#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>


template <typename F>
auto& operator<<(const std::unique_ptr<cairo_t, decltype(&cairo_destroy)>& cairo, F&& manip)
{
  return manip(cairo);
}


template <typename C = char>
auto& operator<<(const std::unique_ptr<cairo_t, decltype(&cairo_destroy)>& cairo,
                 const C* utf8)
{
  cairo_show_text(cairo.get(), utf8);
  return cairo;
}


template <typename C = char>
auto& operator<<(const std::unique_ptr<cairo_t, decltype(&cairo_destroy)>& cairo,
                 const std::basic_string<C>& utf8)
{
  cairo_show_text(cairo.get(), utf8.c_str());
  return cairo;
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


auto color = [](auto&& r, auto&& g, auto&& b, double&& a = 1.0)
  -> auto
{
  return [&](auto& cairo) -> auto&
         {
           cairo_set_source_rgba(cairo.get(), std::forward<decltype(r)>(r), std::forward<decltype(g)>(g), std::forward<decltype(b)>(b), std::forward<decltype(a)>(a));
           return cairo;
         };
};


auto face = [](const std::string& family)
  -> auto
{
  return [&](auto& cairo) -> auto&
         {
           cairo_select_font_face(cairo.get(), family.c_str(), CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
           return cairo;
         };
};


auto size = [](auto&& size)
  -> auto
{
  return [&](auto& cairo) -> auto&
         {
           cairo_set_font_size(cairo.get(), std::forward<decltype(size)>(size));
           return cairo;
         };
};


auto move = [](auto&& x, auto&& y)
  -> auto
{
  return [&](auto& cairo) -> auto&
         {
           cairo_move_to(cairo.get(), std::forward<decltype(x)>(x), std::forward<decltype(y)>(y));
           return cairo;
         };
};


} // namespace meevax


#endif
