#ifndef INCLUDED_MEEVAX_VSTREAM_GRAPHIX_OPERATOR_HPP_
#define INCLUDED_MEEVAX_VSTREAM_GRAPHIX_OPERATOR_HPP_


#include <regex>
#include <string>
#include <vector>
#include <utility>

#include <cairo/cairo-xlib.h>

#include <meevax/algorithm/regex_split_include_delimiter.hpp>
#include <meevax/vstream/graphix_manipulator.hpp>


namespace meevax {


template <typename F>
inline auto operator<<(const meevax::graphix_impl& lhs, F&& rhs)
  -> decltype(rhs(lhs))
{
  return rhs(lhs);
}


template <typename C>
inline auto& operator<<(const meevax::graphix_impl& lhs, const C* rhs)
{
  std::basic_string<C> buffer {rhs};
  return lhs << std::move(buffer);
}


template <typename C>
inline auto& operator<<(const meevax::graphix_impl& lhs, const std::basic_string<C>& rhs)
{
  std::vector<std::basic_string<C>> buffer {};

  static const std::vector<std::basic_regex<C>> delimiters {
    std::basic_regex<C> {"\\\e\\[(\\d*;?)+(.)"}, // escape sequence
    std::basic_regex<C> {"\n"}
  };

  meevax::algorithm::regex_split_include_delimiter(buffer, rhs, delimiters);

  buffer.erase(
    std::remove_if(std::begin(buffer), std::end(buffer), [](auto s) { return s.empty(); }),
    std::end(buffer)
  );

  return lhs << std::move(buffer);
}


template <typename C>
inline auto& operator<<(const meevax::graphix_impl& lhs, const std::vector<std::basic_string<C>>& rhs)
{
  static std::match_results<
    typename std::basic_string<C>::const_iterator
  > results;

  static const std::basic_regex<C>  crlf_regex {"^(\n)$"};
  static const std::basic_regex<C> color_regex {"^(\\\e\\[(\\d+);(\\d+);(\\d+)([Mm]))$"};

  for (const auto& s : rhs)
  {
    if (std::regex_match(s, results, crlf_regex))
    {
      lhs << meevax::cr << meevax::lf;
    }

    else if (std::regex_match(s, results, color_regex))
    {
      lhs << meevax::color<std::uint8_t>(
        std::stoi(results[2]), std::stoi(results[3]), std::stoi(results[4])
      );

      if (results[5] == 'M') { lhs << meevax::paint; }
    }

    else
    {
      if (!s.empty())
      {
        cairo_show_text(static_cast<cairo_t*>(lhs), std::basic_string<C> {s}.c_str());
        continue;
      }
    }
  }

  return lhs;
}


} // namespace meevax


#endif
