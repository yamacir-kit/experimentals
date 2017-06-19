#ifndef INCLUDED_MEEVAX_VSTREAM_VSTREAM_MANIPULATOR_HPP_
#define INCLUDED_MEEVAX_VSTREAM_VSTREAM_MANIPULATOR_HPP_


#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>


namespace meevax {


auto map = [](auto& cairo)
  -> auto&
{
  XMapWindow(
    cairo_xlib_surface_get_display(cairo_get_target(cairo.get())),
    cairo_xlib_surface_get_drawable(cairo_get_target(cairo.get()))
  );
  return cairo;
};


auto raise = [](auto& cairo)
  -> auto&
{
  XMapRaised(
    cairo_xlib_surface_get_display(cairo_get_target(cairo.get())),
    cairo_xlib_surface_get_drawable(cairo_get_target(cairo.get()))
  );
  return cairo;
};


auto unmap = [](auto& cairo)
  -> auto&
{
  XUnmapWindow(
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


auto paint = [](auto& cairo)
  -> auto&
{
  cairo_paint(cairo.get());
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


auto cursorhome = [](auto& cairo)
  -> auto&
{
  static cairo_text_extents_t extents {};
  cairo_text_extents(cairo.get(), "hoge", &extents);

  cairo_move_to(cairo.get(), 0, extents.height);

  return cairo;
};


auto cr = [](auto& cairo)
  -> auto&
{
  // static cairo_text_extents_t extents {};
  // cairo_text_extents(cairo.get(), "hoge", &extents);

  static double x {0}, y {0};
  if (cairo_has_current_point(cairo.get()))
  {
    cairo_get_current_point(cairo.get(), &x, &y);
  }

  cairo_move_to(cairo.get(), 0, y);

  return cairo;
};


auto lf = [](auto& cairo)
  -> auto&
{
  static cairo_text_extents_t extents {};
  cairo_text_extents(cairo.get(), "hoge", &extents);

  static double x {0}, y {0};
  if (cairo_has_current_point(cairo.get()))
  {
    cairo_get_current_point(cairo.get(), &x, &y);
  }

  cairo_move_to(cairo.get(), x, y + extents.height + 3);

  return cairo;
};


auto endl = [](auto& cairo)
  -> auto&
{
  static cairo_text_extents_t extents {};
  cairo_text_extents(cairo.get(), "hoge", &extents);

  static double x {0}, y {0};
  if (cairo_has_current_point(cairo.get()))
  {
    cairo_get_current_point(cairo.get(), &x, &y);
  }

  cairo_move_to(cairo.get(), 0, y + extents.height);

  return flush(cairo);
};


} // namespace meevax


#endif
