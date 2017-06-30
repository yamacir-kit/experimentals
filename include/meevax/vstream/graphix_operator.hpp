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
  boost::algorithm::split(buffer, rhs, boost::is_any_of("\e"));

#ifndef NDEBUG
  std::cout << "[debug] size: " << buffer.size() << ", \e[1;31m^\e[0m";
  for (const auto& s : buffer)
  {
    std::cout << s << (&s != &buffer.back() ? "\e[1;31m\\e\e[0m" : "\e[1;31m$\e[0m\n");
  }
#endif

  return lhs << buffer;
}


template <typename C>
auto& print(const meevax::graphix_impl& lhs, const std::basic_string<C>& rhs)
{
  std::vector<std::basic_string<C>> buffer {};
  boost::algorithm::split(buffer, rhs, boost::is_any_of("\n"));

  for (const auto& s : buffer)
  {
    cairo_show_text(static_cast<cairo_t*>(lhs), s.c_str());
    if (&s != &buffer.back()) { lhs << meevax::cr << meevax::lf; }
  }

  return lhs;
}


template <typename C>
auto& operator<<(const meevax::graphix_impl& lhs, const std::vector<std::basic_string<C>>& rhs)
{
  if (!rhs.front().empty())
  {
    meevax::print(lhs, rhs.front());
  }

  static std::match_results<typename std::basic_string<C>::const_iterator> results;

  for (auto iter {rhs.begin() + 1}; iter != rhs.end(); ++iter)
  {
    if (std::regex_match(*iter, results, std::basic_regex<C> {"^\\[([0-9xXa-fA-F]+);([0-9xXa-fA-F]+);([0-9xXa-fA-F]+)fg.*$"}))
    {
      lhs << meevax::color<std::uint8_t>(
        std::stoi(results[1], nullptr, 16),
        std::stoi(results[2], nullptr, 16),
        std::stoi(results[3], nullptr, 16)
      );

      print(
        lhs,
        std::basic_string<C> {
          (*iter).begin() + results[1].length()
                          + results[2].length()
                          + results[3].length() + 5,
          (*iter).end()
        }
      );
    }

    else if (std::regex_match(*iter, results, std::basic_regex<C> {"^\\[([0-9xXa-fA-F]+);([0-9xXa-fA-F]+);([0-9xXa-fA-F]+)bg.*$"}))
    {
      lhs << meevax::color<std::uint8_t>(
        std::stoi(results[1], nullptr, 16),
        std::stoi(results[2], nullptr, 16),
        std::stoi(results[3], nullptr, 16)
      ) << meevax::paint;

      print(
        lhs,
        std::basic_string<C> {
          (*iter).begin() + results[1].length()
                          + results[2].length()
                          + results[3].length() + 5,
          (*iter).end()
        }
      );
    }

    else
    {
      // throw std::runtime_error {"unexpected escape sequence: \\e" + *iter};
      meevax::print(lhs, *iter);
    }
  }

  return lhs;
}


} // namespace meevax


#endif
