#ifndef INCLUDED_MEEVAX_VSTREAM_GRAPHIX_OPERATOR_HPP_
#define INCLUDED_MEEVAX_VSTREAM_GRAPHIX_OPERATOR_HPP_


#include <regex>
#include <string>
#include <vector>

#include <cairo/cairo-xlib.h>

// #include <boost/algorithm/string/split.hpp>
// #include <boost/algorithm/string/classification.hpp>

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

#ifndef NDEBUG
  std::cout << "[debug] size: " << buffer.size() << ", \e[1;31m^\e[0m";
  for (const auto& s : buffer)
  {
    if (s.front() == '\e')
    {
      std::cout << "\e[1;33m\\e\e[0m[" << std::string {s.begin() + 2, s.end()}
                << (&s != &buffer.back() ? "\e[1;36m/\e[0m" : "\e[1;31m$\e[0m\n");
    }
    else if (s.front() == '\n')
    {
      std::cout << "\e[1;33m\\n\e[0m" << std::string {s.begin() + 1, s.end()}
                << (&s != &buffer.back() ? "\e[1;36m/\e[0m" : "\e[1;31m$\e[0m\n");
    }
    else
    {
      std::cout << s
                << (&s != &buffer.back() ? "\e[1;36m/\e[0m" : "\e[1;31m$\e[0m\n");
    }
  }
#endif

  return lhs << buffer;
}


// template <typename C>
// auto& print(const meevax::graphix_impl& lhs, const std::basic_string<C>& rhs)
// {
//   std::vector<std::basic_string<C>> buffer {};
//   boost::algorithm::split(buffer, rhs, boost::is_any_of("\n"));
//
//   for (const auto& s : buffer)
//   {
//     cairo_show_text(static_cast<cairo_t*>(lhs), s.c_str());
//     if (&s != &buffer.back()) { lhs << meevax::cr << meevax::lf; }
//   }
//
//   return lhs;
// }


template <typename C>
auto& operator<<(const meevax::graphix_impl& lhs, const std::vector<std::basic_string<C>>& rhs)
{
  // if (!rhs.front().empty())
  // {
  //   cairo_show_text(static_cast<cairo_t*>(lhs), rhs.front().c_str());
  // }

  static std::match_results<typename std::basic_string<C>::const_iterator> results;

  for (auto iter {rhs.begin()}; iter != rhs.end(); ++iter)
  {
    if (std::regex_match(*iter, results, std::basic_regex<C> {"^(\n)(.*)$"}))
    {
      // std::cout << "[debug] \e[1;31m^\e[1;36m" << "\\n"
      //                            << "\e[1;33m" << results[2] << "\e[1;31m$\e[0m\n";

      lhs << meevax::cr << meevax::lf;

      cairo_show_text(
        static_cast<cairo_t*>(lhs),
        std::basic_string<C> {(*iter).begin() + results[1].length(), (*iter).end()}.c_str()
      );
    }

    else if (std::regex_match(*iter, results, std::basic_regex<C> {
               "^(\\\e\\[([0-9xXa-fA-F]+);([0-9xXa-fA-F]+);([0-9xXa-fA-F]+)fg)(.*)$"
            }))
    {
      // std::cout << "[debug] \e[1;31m^\e[1;36m" << results[1]
      //                            << "\e[1;33m" << results[2] << "\e[1;31m$\e[0m\n";

      lhs << meevax::color<std::uint8_t>(
        std::stoi(results[2], nullptr, 16),
        std::stoi(results[3], nullptr, 16),
        std::stoi(results[4], nullptr, 16)
      );

      cairo_show_text(
        static_cast<cairo_t*>(lhs),
        std::basic_string<C> {(*iter).begin() + results[1].length(), (*iter).end()}.c_str()
      );
    }

    else if (std::regex_match(*iter, results, std::basic_regex<C> {
               "^(\\\e\\[([0-9xXa-fA-F]+);([0-9xXa-fA-F]+);([0-9xXa-fA-F]+)bg).*$"
            }))
    {
      // std::cout << "[debug] \e[1;31m^\e[1;36m" << results[1]
      //                            << "\e[1;33m" << results[2] << "\e[1;31m$\e[0m\n";

      lhs << meevax::color<std::uint8_t>(
        std::stoi(results.str(2), nullptr, 16),
        std::stoi(results.str(3), nullptr, 16),
        std::stoi(results.str(4), nullptr, 16)
      ) << meevax::paint;

      cairo_show_text(
        static_cast<cairo_t*>(lhs),
        std::basic_string<C> {(*iter).begin() + results[1].length(), (*iter).end()}.c_str()
      );
    }

    else
    {
      if (!(*iter).empty())
      {
        // throw std::runtime_error {"unexpected escape sequence: \\e" + *iter};
        cairo_show_text(static_cast<cairo_t*>(lhs), (*iter).c_str());
      }
    }
  }

  return lhs;
}


} // namespace meevax


#endif
