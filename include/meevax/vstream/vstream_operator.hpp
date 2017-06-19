#ifndef INCLUDED_MEEVAX_VSTREAM_VSTREAM_OPERATOR_HPP_
#define INCLUDED_MEEVAX_VSTREAM_VSTREAM_OPERATOR_HPP_


#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>


template <typename F>
auto& operator<<(const std::unique_ptr<cairo_t, decltype(&cairo_destroy)>& cairo, F&& manip)
{
  return manip(cairo);
}


template <typename C = char>
auto& operator<<(const std::unique_ptr<cairo_t, decltype(&cairo_destroy)>& cairo,
                 std::vector<std::vector<std::basic_string<C>>>&& text)
{
  for (const auto& line : text)
  {
    for (const auto& word : line)
    {
      cairo_show_text(cairo.get(), (&word != &line.back() ? word + " " : word).c_str());
    }
    if (&line != &text.back()) { cairo << meevax::endl; }
  }

  return cairo;
}


template <typename C = char>
auto& operator<<(const std::unique_ptr<cairo_t, decltype(&cairo_destroy)>& cairo,
                 const C* utf8)
{
  std::basic_stringstream<C> text {utf8};

  std::vector<std::vector<std::basic_string<C>>> parsive {};
              std::vector<std::basic_string<C>>  line_buffer {};

  for (std::basic_string<C> buffer {}; std::getline(text, buffer); parsive.push_back(line_buffer))
  {
    for (std::basic_stringstream<C> line {buffer}; std::getline(line, buffer, ' '); line_buffer.push_back(buffer));
  }

  return cairo << std::move(parsive);
}


#endif
