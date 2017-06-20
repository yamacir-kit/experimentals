#ifndef INCLUDED_MEEVAX_VSTREAM_VSTREAM_OPERATOR_HPP_
#define INCLUDED_MEEVAX_VSTREAM_VSTREAM_OPERATOR_HPP_


#include <memory>
#include <string>

#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>


template <typename F>
[[deprecated]] auto& operator<<(const std::unique_ptr<cairo_t, decltype(&cairo_destroy)>& cairo, F&& manip)
{
  return manip(cairo);
}


template <typename C>
[[deprecated]] auto& operator<<(const std::unique_ptr<cairo_t, decltype(&cairo_destroy)>& cairo, C* c_str)
{
  cairo_show_text(cairo.get(), c_str);
  return cairo;
};


template <typename C>
[[deprecated]] auto& operator<<(const std::unique_ptr<cairo_t, decltype(&cairo_destroy)>& cairo,
                 const std::basic_string<C>& text)
{
  cairo_show_text(cairo.get(), text.c_str());
  return cairo;
};


#endif
