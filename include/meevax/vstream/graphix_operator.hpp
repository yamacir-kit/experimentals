#ifndef INCLUDED_MEEVAX_VSTREAM_GRAPHIX_OPERATOR_HPP_
#define INCLUDED_MEEVAX_VSTREAM_GRAPHIX_OPERATOR_HPP_


#include <regex>
#include <string>
#include <vector>

#include <cairo/cairo-xlib.h>

#include <meevax/algorithm/split_include_delimiter.hpp>
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
  meevax::algorithm::split_include_delimiter(buffer, rhs, std::basic_string<C> {"\e\n"});

  return lhs << buffer;
}


template <typename C>
auto& operator<<(const meevax::graphix_impl& lhs, const std::vector<std::basic_string<C>>& rhs)
{
  static std::match_results<typename std::basic_string<C>::const_iterator> results;

  for (auto iter {rhs.begin()}; iter != rhs.end(); ++iter)
  {
    if (std::regex_match(*iter, results, std::basic_regex<C> {"^(\n)(.*)$"}))
    {
      lhs << meevax::cr << meevax::lf;

      cairo_show_text(
        static_cast<cairo_t*>(lhs),
        std::basic_string<C> {(*iter).begin() + results[1].length(), (*iter).end()}.c_str()
      );
    }

    else if (std::regex_match(*iter, results, std::basic_regex<C> {
               "^(\\\e\\[([0-9xXa-fA-F]+);([0-9xXa-fA-F]+);([0-9xXa-fA-F]+)([fb])g)(.*)$"
            }))
    {
      lhs << meevax::color<std::uint8_t>(
        std::stoi(results[2], nullptr, 16),
        std::stoi(results[3], nullptr, 16),
        std::stoi(results[4], nullptr, 16)
      );

      if (results[5] == 'b')
      {
        lhs << meevax::paint;
      }

      cairo_show_text(
        static_cast<cairo_t*>(lhs),
        std::basic_string<C> {(*iter).begin() + results[1].length(), (*iter).end()}.c_str()
      );
    }

    else
    {
      if (!(*iter).empty())
      {
        cairo_show_text(static_cast<cairo_t*>(lhs), (*iter).c_str());
      }
    }
  }

  return lhs;
}


} // namespace meevax


#endif
