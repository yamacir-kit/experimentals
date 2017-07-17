#ifndef INCLUDED_MEEVAX_VSTREAM_GRAPHIX_MANIPULATOR_HPP_
#define INCLUDED_MEEVAX_VSTREAM_GRAPHIX_MANIPULATOR_HPP_


#include <limits>

#include <cairo/cairo-xlib.h>


namespace meevax {


[[deprecated]] auto flush = [](auto& node) -> auto&
{
  cairo_surface_flush(static_cast<cairo_surface_t*>(node));
  return node;
};


auto paint = [](auto& node) -> auto&
{
  cairo_paint(static_cast<cairo_t*>(node));
  return node;
};


template <typename T>
constexpr auto color(T&& r, T&& g, T&& b, T&& a = std::numeric_limits<T>::max())
{
  return [&](auto& node) -> auto&
  {
    cairo_set_source_rgba(
      static_cast<cairo_t*>(node),
      std::forward<double>(r / static_cast<double>(std::numeric_limits<T>::max())),
      std::forward<double>(g / static_cast<double>(std::numeric_limits<T>::max())),
      std::forward<double>(b / static_cast<double>(std::numeric_limits<T>::max())),
      std::forward<double>(a / static_cast<double>(std::numeric_limits<T>::max()))
    );
    return node;
  };
}


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


auto move = [](auto&& x, auto&& y)
{
  return [&](auto& node) -> auto&
  {
    cairo_move_to(static_cast<cairo_t*>(node), std::forward<decltype(x)>(x), std::forward<decltype(y)>(y));
    return node;
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
