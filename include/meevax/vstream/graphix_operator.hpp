#ifndef INCLUDED_MEEVAX_VSTREAM_GRAPHIX_OPERATOR_HPP_
#define INCLUDED_MEEVAX_VSTREAM_GRAPHIX_OPERATOR_HPP_


#include <regex>
#include <string>
#include <vector>

#include <cairo/cairo-xlib.h>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <meevax/vstream/graphix_manipulator.hpp>


namespace meevax {


template <typename F>
auto operator<<(const meevax::graphix_impl& lhs, F&& rhs)
  -> decltype(rhs(lhs))
{
  return rhs(lhs);
}


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
  boost::algorithm::split(buffer, rhs, boost::is_any_of("\\"));

  return lhs << buffer;
}


template <typename C>
auto& operator<<(const meevax::graphix_impl& lhs, const std::vector<std::basic_string<C>>& rhs)
{
  cairo_show_text(static_cast<cairo_t*>(lhs), rhs.front().c_str());

  static std::match_results<typename std::basic_string<C>::const_iterator> results;

  for (auto iter {rhs.begin() + 1}; iter != rhs.end(); ++iter)
  {
    if (std::regex_match(*iter, std::basic_regex<C> {"^n.*$"}))
    {
      lhs << meevax::cr
          << meevax::lf; // TODO CR+LF to be CRLF

      cairo_show_text(
        static_cast<cairo_t*>(lhs),
        std::basic_string<C> {(*iter).begin() + 1, (*iter).end()}.c_str()
      );
    }

    else if (std::regex_match(*iter, results, std::basic_regex<C> {"^e\\[([0-9xXa-fA-F]+);([0-9xXa-fA-F]+);([0-9xXa-fA-F]+)fgc.*$"}))
    {
      lhs << meevax::color<std::uint8_t>(
        std::stoi(results[1], nullptr, 16),
        std::stoi(results[2], nullptr, 16),
        std::stoi(results[3], nullptr, 16)
      );

      cairo_show_text(
        static_cast<cairo_t*>(lhs),
        std::basic_string<C> {
          (*iter).begin() + results[1].length()
                          + results[2].length()
                          + results[3].length() + 7,
          (*iter).end()
        }.c_str()
      );
    }

    else if (std::regex_match(*iter, results, std::basic_regex<C> {"^e\\[([0-9xXa-fA-F]+);([0-9xXa-fA-F]+);([0-9xXa-fA-F]+)bgc.*$"}))
    {
      lhs << meevax::color<std::uint8_t>(
        std::stoi(results[1], nullptr, 16),
        std::stoi(results[2], nullptr, 16),
        std::stoi(results[3], nullptr, 16)
      ) << meevax::paint;

      cairo_show_text(
        static_cast<cairo_t*>(lhs),
        std::basic_string<C> {
          (*iter).begin() + results[1].length()
                          + results[2].length()
                          + results[3].length() + 7,
          (*iter).end()
        }.c_str()
      );
    }

    else
    {
      throw std::runtime_error {"unexpected escape sequence: " + *iter};
    }
  }

  return lhs;
}


} // namespace meevax


#endif
