#ifndef INCLUDED_MEEVAX_VSTREAM_GRAPHIX_OPERATOR_HPP_
#define INCLUDED_MEEVAX_VSTREAM_GRAPHIX_OPERATOR_HPP_


#include <regex>
#include <string>
#include <vector>
#include <utility>

#include <cairo/cairo-xlib.h>

// #include <meevax/algorithm/split_include_delimiter.hpp>
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
  std::string debug;

  // std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
  std::vector<std::basic_string<C>> buffer {};

  // std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
  static const std::vector<std::basic_regex<C>> delimiters {
    // std::basic_regex<C> {"\\\e\\[(\\d*;?)+(.)"}, // escape sequence
    std::basic_regex<C> {"\\\e\\[(\\d*;?)+(.)"}, // escape sequence
    std::basic_regex<C> {"\n"}
  };

  // std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
  meevax::algorithm::regex_split_include_delimiter(
      buffer,
    rhs,
    std::forward<decltype(delimiters)>(delimiters));

  buffer.erase(std::remove_if(std::begin(buffer), std::end(buffer), [](auto s) {
    return s.empty();
  }), std::end(buffer));

  for (const auto& s : buffer)
  {
    if (s[0] == '\e')
    {
      std::cout << "\\e" << std::string {std::begin(s) + 1, std::end(s)};
    }
    else if (s[0] == '\n')
    {
      std::cout << "\\n" << std::string {std::begin(s) + 1, std::end(s)};
    }
    else
    {
      std::cout << s;
    }

    std::cout << "\e[1;31m_\e[0m";
  }
  std::cout << '\n';

  std::getline(std::cin, debug);

  return lhs << std::move(buffer);
}


template <typename C>
inline auto& operator<<(const meevax::graphix_impl& lhs, const std::vector<std::basic_string<C>>& rhs)
{
  static std::match_results<
    typename std::basic_string<C>::const_iterator
  > results;

  static const std::basic_regex<C>  crlf_regex {"^(\n)$"};
  static const std::basic_regex<C> color_regex {"^(\\\e\\[([0-9xXa-fA-F]+);([0-9xXa-fA-F]+);([0-9xXa-fA-F]+)([Mm]))$"};

  for (const auto& s : rhs)
  {
    std::cout << "[debug] regex search: ";
    if (s[0] == '\e')
    {
      std::cout << "\\e" << std::string {std::begin(s) + 1, std::end(s)};
    }
    else if (s[0] == '\n')
    {
      std::cout << "\\n" << std::string {std::begin(s) + 1, std::end(s)};
    }
    else
    {
      std::cout << s;
    }
    std::cout << " -> ";

    if (std::regex_match(s, results, crlf_regex))
    {
      std::cout << "match: crlf\n";
      lhs << meevax::cr << meevax::lf;
    }

    else if (std::regex_match(s, results, color_regex))
    {
      std::cout << "match: color\n";
      lhs << meevax::color<std::uint8_t>(
        std::stoi(results[2]),
        std::stoi(results[3]),
        std::stoi(results[4])
      );

      if (results[5] == 'M') { lhs << meevax::paint; }
    }

    else if (!s.empty())
    {
      std::cout << "match: plain text\n";
      cairo_show_text(static_cast<cairo_t*>(lhs), std::basic_string<C> {s}.c_str());
      continue;
    }

    // cairo_show_text(
    //   static_cast<cairo_t*>(lhs),
    //   std::basic_string<C> {s.begin() + results[1].length(), s.end()}.c_str()
    // );
  }
  std::cout << "\n";

  return lhs;
}


} // namespace meevax


#endif
