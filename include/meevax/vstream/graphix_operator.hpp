#ifndef INCLUDED_MEEVAX_VSTREAM_GRAPHIX_OPERATOR_HPP_
#define INCLUDED_MEEVAX_VSTREAM_GRAPHIX_OPERATOR_HPP_


#include <string>
#include <vector>

#include <cairo/cairo-xlib.h>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>


namespace meevax {


template <typename F>
auto operator<<(const meevax::graphix_impl& lhs, F&& rhs)
  -> decltype(rhs(lhs))
{
  return rhs(lhs);
}


// template <typename C>
// auto& operator<<(const meevax::graphix_impl& lhs, C* rhs)
// {
//   std::basic_string<C> buffer {rhs};
//   return lhs << std::forward<decltype(buffer)>(buffer);
// }


template <typename C>
auto& operator<<(const meevax::graphix_impl& lhs, const C* rhs)
{
  std::basic_string<C> buffer {rhs};
  return lhs << std::forward<decltype(buffer)>(buffer);
}


template <typename C>
auto& operator<<(const meevax::graphix_impl& lhs, const std::basic_string<C>& rhs)
{
  std::vector<std::basic_string<C>> buffer {};
  boost::algorithm::split(buffer, rhs, boost::is_any_of(" "));

  return lhs << buffer;
}


template <typename C>
auto& operator<<(const meevax::graphix_impl& lhs, const std::vector<std::basic_string<C>>& rhs)
{
  for (const auto& s : rhs)
  {
    cairo_show_text(static_cast<cairo_t*>(lhs), (&s != &rhs.back() ? std::basic_string<C> {s + " "} : s).c_str());
  }

  return lhs;
}


} // namespace meevax


#endif
