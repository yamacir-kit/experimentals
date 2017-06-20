#ifndef INCLUDED_MEEVAX_VSTREAM_VSTREAM_MANIPULATOR_HPP_
#define INCLUDED_MEEVAX_VSTREAM_VSTREAM_MANIPULATOR_HPP_


#include <meevax/vstream/basic_vstream.hpp>


namespace meevax {


// auto xmove = [](auto&& x, auto&& y)
// {
//   return [&](auto& node) -> auto&
//   {
//     XMoveWindow(static_cast<Display*>(node), static_cast<Window>(node), std::forward<decltype(x)>(x), std::forward<decltype(y)>(y));
//     return node;
//   };
// };


auto map = [](auto& node) -> auto&
{
  XMapWindow(static_cast<Display*>(node), static_cast<Window>(node));
  return node;
};


auto raise = [](auto& node) -> auto&
{
  XMapRaised(static_cast<Display*>(node), static_cast<Window>(node));
  return node;
};


auto unmap = [](auto& node) -> auto&
{
  XUnmapWindow(static_cast<Display*>(node), static_cast<Window>(node));
  return node;
};


auto flush = [](auto& node) -> auto&
{
  cairo_surface_flush(static_cast<cairo_surface_t*>(node));
  XFlush(static_cast<Display*>(node));
  return node;
};


auto paint = [](auto& node) -> auto&
{
  cairo_paint(static_cast<cairo_t*>(node));
  return node;
};


auto color = [](auto&& r, auto&& g, auto&& b, double&& a = 1.0)
{
  return [&](auto& node) -> auto&
  {
    cairo_set_source_rgba(static_cast<cairo_t*>(node), std::forward<decltype(r)>(r), std::forward<decltype(g)>(g), std::forward<decltype(b)>(b), std::forward<decltype(a)>(a));
    return node;
  };
};


auto face = [](const std::string& family)
{
  return [&](auto& node) -> auto&
  {
    cairo_select_font_face(static_cast<cairo_t*>(node), family.c_str(), CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    return node;
  };
};


auto size = [](auto&& size)
{
  return [&](auto& node) -> auto&
  {
    cairo_set_font_size(static_cast<cairo_t*>(node), std::forward<decltype(size)>(size));
    return node;
  };
};


[[deprecated]] auto move = [](auto&& x, auto&& y)
{
  return [&](auto& node) -> auto&
  {
    cairo_move_to(static_cast<cairo_t*>(node), std::forward<decltype(x)>(x), std::forward<decltype(y)>(y));
    return node;
  };
};


auto cursorhome = [](auto& node) -> auto&
{
  static cairo_text_extents_t extents {};
  cairo_text_extents(static_cast<cairo_t*>(node), "hoge", &extents);
  cairo_move_to(static_cast<cairo_t*>(node), 0, extents.height);
  return node;
};


auto cr = [](auto& node) -> auto&
{
  static double x {0}, y {0};
  if (cairo_has_current_point(static_cast<cairo_t*>(node)))
  {
    cairo_get_current_point(static_cast<cairo_t*>(node), &x, &y);
  }

  cairo_move_to(static_cast<cairo_t*>(node), 0, y);

  return node;
};


auto lf = [](auto& node) -> auto&
{
  static cairo_text_extents_t extents {};
  cairo_text_extents(static_cast<cairo_t*>(node), "hoge", &extents);

  static double x {0}, y {0};
  if (cairo_has_current_point(static_cast<cairo_t*>(node)))
  {
    cairo_get_current_point(static_cast<cairo_t*>(node), &x, &y);
  }

  cairo_move_to(static_cast<cairo_t*>(node), x, y + extents.height + 3);

  return node;
};


auto endl = [](auto& node) -> auto&
{
  static cairo_text_extents_t extents {};
  cairo_text_extents(static_cast<cairo_t*>(node), "hoge", &extents);

  static double x {0}, y {0};
  if (cairo_has_current_point(static_cast<cairo_t*>(node)))
  {
    cairo_get_current_point(static_cast<cairo_t*>(node), &x, &y);
  }

  cairo_move_to(static_cast<cairo_t*>(node), 0, y + extents.height);

  return flush(node);
};


auto resize = [](auto&& width = 0, auto&& height = 0)
{
  return [&](auto& node) -> auto&
  {
    XWindowAttributes attr {};
    XGetWindowAttributes(static_cast<Display*>(node), static_cast<Window>(node), &attr);

    XResizeWindow(
      static_cast<Display*>(node), static_cast<Window>(node),
      std::forward<decltype(width)>(width != 0 ? width : attr.width),
      std::forward<decltype(height)>(height != 0 ? height : attr.height)
    );

    cairo_xlib_surface_set_size(static_cast<cairo_surface_t*>(node), attr.width, attr.height);

    return node;
  };
};


} // namespace meevax


#endif
