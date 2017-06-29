#ifndef INCLUDED_MEEVAX_VSTREAM_GRAPHIX_OPERATOR_HPP_
#define INCLUDED_MEEVAX_VSTREAM_GRAPHIX_OPERATOR_HPP_


#include <cairo/cairo-xlib.h>


namespace meevax {


template <typename F>
auto& operator<<(const meevax::graphix_impl& impl, F&& f)
{
  return f(impl);
}


template <typename C>
auto& operator<<(const meevax::graphix_impl& impl, C* c_str)
{
  cairo_show_text(static_cast<cairo_t*>(impl), c_str);
  return impl;
}


template <typename C>
auto& operator<<(const meevax::graphix_impl& impl, const C* c_str)
{
  cairo_show_text(static_cast<cairo_t*>(impl), c_str);
  return impl;
}


template <typename C>
auto& operator<<(const meevax::graphix_impl& impl, const std::basic_string<C>& text)
{
  cairo_show_text(static_cast<cairo_t*>(impl), text.c_str());
  return impl;
}


} // namespace meevax


#endif
