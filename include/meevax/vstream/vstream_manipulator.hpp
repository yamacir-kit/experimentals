#ifndef INCLUDED_MEEVAX_VSTREAM_VSTREAM_MANIPULATOR_HPP_
#define INCLUDED_MEEVAX_VSTREAM_VSTREAM_MANIPULATOR_HPP_


#include <limits>

#include <cairo/cairo-xlib.h>


namespace meevax {


auto flush = [](auto& lhs) -> auto&
{
  cairo_surface_flush(static_cast<cairo_surface_t*>(lhs));
  return lhs;
};


auto paint = [](auto& lhs) -> auto&
{
  cairo_paint(static_cast<cairo_t*>(lhs));
  return lhs;
};


template <typename T>
auto color(T&& r, T&& g, T&& b, T&& a = std::numeric_limits<T>::max())
{
  return [&](auto& lhs) -> auto&
  {
    cairo_set_source_rgba(
      static_cast<cairo_t*>(lhs),
      std::forward<double>(static_cast<double>(r) / std::numeric_limits<T>::max()),
      std::forward<double>(static_cast<double>(g) / std::numeric_limits<T>::max()),
      std::forward<double>(static_cast<double>(b) / std::numeric_limits<T>::max()),
      std::forward<double>(static_cast<double>(a) / std::numeric_limits<T>::max())
    );
    return lhs;
  };
}


auto face = [](const std::string& family)
{
  return [&](auto& lhs) -> auto&
  {
    cairo_select_font_face(static_cast<cairo_t*>(lhs), family.c_str(), CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    return lhs;
  };
};


auto size = [](auto&& size)
{
  return [&](auto& lhs) -> auto&
  {
    cairo_set_font_size(static_cast<cairo_t*>(lhs), std::forward<decltype(size)>(size));
    return lhs;
  };
};


auto move = [](auto&& x, auto&& y)
{
  return [&](auto& lhs) -> auto&
  {
    cairo_move_to(static_cast<cairo_t*>(lhs), std::forward<decltype(x)>(x), std::forward<decltype(y)>(y));
    return lhs;
  };
};


auto cursorhome = [](auto& lhs) -> auto&
{
  cairo_move_to(static_cast<cairo_t*>(lhs), 0, lhs.extents(std::string {"hoge"}).height);
  return lhs;
};


auto cr = [](auto& node) -> auto&
{
  static double x, y;
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

  static double x, y;
  if (cairo_has_current_point(static_cast<cairo_t*>(node)))
  {
    cairo_get_current_point(static_cast<cairo_t*>(node), &x, &y);
  }

  cairo_move_to(static_cast<cairo_t*>(node), x, y + extents.height + 3);

  return node;
};


[[deprecated]] auto endl = [](auto& node) -> auto&
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


} // namespace meevax


#endif
