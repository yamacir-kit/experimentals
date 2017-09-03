#ifndef INCLUDED_MEEVAX_VSTREAM_VSTREAM_OPERATOR_HPP_
#define INCLUDED_MEEVAX_VSTREAM_VSTREAM_OPERATOR_HPP_


#include <regex>
#include <string>
#include <utility>
#include <vector>

#include <cairo/cairo-xlib.h>

#include <meevax/algorithm/regex_split_include_delimiter.hpp>
#include <meevax/vstream/vstream_manipulator.hpp>


namespace meevax {


template <typename C, typename F>
[[deprecated]] inline auto operator<<(const meevax::basic_vstream<C>& lhs, F&& rhs)
  -> decltype(rhs(lhs))
{
  return rhs(lhs);
}


template <typename C>
[[deprecated]] inline auto& operator<<(const meevax::basic_vstream<C>& lhs, const C* rhs)
{
  std::basic_string<C> buffer {rhs};
  return lhs << std::move(buffer);
}


template <typename C>
[[deprecated]] inline auto& operator<<(const meevax::basic_vstream<C>& lhs, const std::basic_string<C>& rhs)
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
[[deprecated]] inline auto& operator<<(const meevax::basic_vstream<C>& lhs, const std::vector<std::basic_string<C>>& rhs)
{
  static std::match_results<
    typename std::basic_string<C>::const_iterator
  > results;

  static const std::basic_regex<C>      crlf_regex {"^\n$"};
  static const std::basic_regex<C> rgb_color_regex {"^\\\e\\[(\\d+);(\\d+);(\\d+);(\\d+);(\\d+)m$"};

  for (const auto& s : rhs)
  {
    if (std::regex_match(s, results, crlf_regex))
    {
      lhs << meevax::cr << meevax::lf;
    }

    else if (std::regex_match(s, results, rgb_color_regex)) // XXX UGLY CODE
    {
      if (std::stoi(results[1]) == 38 && std::stoi(results[2]) == 2)
      {
        lhs << meevax::color<std::uint8_t>(
          std::stoi(results[3]), std::stoi(results[4]), std::stoi(results[5])
        );
      }
      else if (std::stoi(results[1]) == 48 && std::stoi(results[2]) == 2)
      {
        lhs << meevax::color<std::uint8_t>(
          std::stoi(results[3]), std::stoi(results[4]), std::stoi(results[5])
        ) << meevax::paint;
      }
      else
      {
        std::basic_string<C> invalid {results[0]};
        invalid.replace(std::begin(invalid), std::begin(invalid) + 1, "\\e");
        cairo_show_text(static_cast<cairo_t*>(lhs), invalid.c_str());
      }
    }

    else
    {
      if (!s.empty())
      {
        std::basic_string<C> plain_text {s};
        if (plain_text[0] == '\e')
        {
          plain_text.replace(std::begin(plain_text), std::begin(plain_text) + 1, "\\e");
        }
        cairo_show_text(static_cast<cairo_t*>(lhs), plain_text.c_str());
      }
    }
  }

  return lhs;
}


} // namespace meevax


#endif
