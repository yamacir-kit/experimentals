#ifndef INCLUDED_MEEVAX_VSTREAM_BASIC_XLIB_VSTREAM_HPP
#define INCLUDED_MEEVAX_VSTREAM_BASIC_XLIB_VSTREAM_HPP


#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <boost/asio/basic_streambuf.hpp>
#include <boost/asio/buffers_iterator.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/point.hpp>

#include <meevax/cairo/surface.hpp>
#include <meevax/type_traits/has_operator.hpp>
#include <meevax/algorithm/regex_split_include_delimiter.hpp>


namespace meevax {


template <typename Char>
class basic_vstream
  : public meevax::cairo::surface,
    public std::basic_iostream<Char>
{
  boost::asio::basic_streambuf<std::allocator<Char>> buffer_;

public:
  template <typename... Ts>
  explicit basic_vstream(Ts&&... args)
    : meevax::cairo::surface {std::forward<Ts>(args)...},
      std::basic_iostream<Char> {&buffer_}
  {}

public:
  auto data()
  {
    return std::basic_string<Char> {boost::asio::buffer_cast<const char*>(buffer_.data())};
  }

  template <typename... Ts>
  decltype(auto) size(Ts&&... args)
  {
    return buffer_.size(std::forward<Ts>(args)...);
  }

  template <typename... Ts>
  decltype(auto) commit(Ts&&... args)
  {
    return buffer_.commit(std::forward<Ts>(args)...);
  }

  template <typename... Ts>
  decltype(auto) prepare(Ts&&... args)
  {
    return buffer_.prepare(std::forward<Ts>(args)...);
  }

  template <typename... Ts>
  decltype(auto) consume(Ts&&... args)
  {
    return buffer_.consume(std::forward<Ts>(args)...);
  }

  [[deprecated]] auto output() // write to window surface
  {
    std::vector<std::basic_string<Char>> buffer {};

    static const std::vector<std::basic_regex<Char>> delimiters {
      std::basic_regex<Char> {"\\\e\\[(\\d*;?)+(.)"}, // escape sequence
      std::basic_regex<Char> {"\n"}
    };

    meevax::algorithm::regex_split_include_delimiter(buffer, data(), delimiters);

    buffer.erase(
      std::remove_if(std::begin(buffer), std::end(buffer), [](auto s) { return s.empty(); }),
      std::end(buffer)
    );

    std::match_results<typename std::basic_string<Char>::const_iterator> results {};

    static const std::basic_regex<Char>  crlf_regex {"^\n$"};
    static const std::basic_regex<Char> color_regex {"^\\\e\\[(\\d+);(\\d+);(\\d+);(\\d+);(\\d+)m$"};

    const std::unique_ptr<cairo_t, decltype(&cairo_destroy)> cairo {
      cairo_create(meevax::cairo::surface::get()), cairo_destroy
    };

    cairo_select_font_face(cairo.get(), "Ricty Diminished", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cairo.get(), 18);

    cairo_text_extents_t extents {};
    cairo_text_extents(cairo.get(), "hoge", &extents);

    cairo_set_source_rgb(cairo.get(), 1.0, 1.0, 1.0);
    cairo_paint(cairo.get());
    cairo_set_source_rgb(cairo.get(), 0.0, 0.0, 0.0);

    cairo_move_to(cairo.get(), 0, extents.height);

    for (const auto& s : buffer)
    {
      if (std::regex_match(s, results, crlf_regex))
      {
        double x, y;
        cairo_get_current_point(cairo.get(), &x, &y);
        boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian> point {x, y};

        static constexpr double line_margin {3};
        cairo_move_to(cairo.get(), 0, boost::geometry::get<1>(point) + extents.height + line_margin);
      }

      else if (std::regex_match(s, results, color_regex)) // XXX UGLY CODE
      {
        if (std::stoi(results[1]) == 38 && std::stoi(results[2]) == 2)
        {
          cairo_set_source_rgb(
            cairo.get(),
            std::stod(results[3]) / std::numeric_limits<std::int8_t>::max(),
            std::stod(results[4]) / std::numeric_limits<std::int8_t>::max(),
            std::stod(results[5]) / std::numeric_limits<std::int8_t>::max()
          );
        }
        else if (std::stoi(results[1]) == 48 && std::stoi(results[2]) == 2)
        {
          cairo_set_source_rgb(
            cairo.get(),
            std::stod(results[3]) / std::numeric_limits<std::int8_t>::max(),
            std::stod(results[4]) / std::numeric_limits<std::int8_t>::max(),
            std::stod(results[5]) / std::numeric_limits<std::int8_t>::max()
          );

          cairo_paint(cairo.get());
        }
        else
        {
          std::basic_string<Char> invalid {results[0]};
          invalid.replace(std::begin(invalid), std::begin(invalid) + 1, "\\e");

          cairo_show_text(cairo.get(), invalid.c_str());
        }
      }

      else
      {
        write(cairo, std::begin(s), std::end(s));
      }
    }
  }

  // void clear()
  // {
  //   const std::unique_ptr<cairo_t, decltype(&cairo_destroy)> context {
  //     cairo_create(meevax::cairo::surface::get()), cairo_destroy
  //   };
  //
  //   cairo_set_source_rgb(context.get(), 1.0, 1.0, 1.0);
  //   cairo_paint(context.get());
  //   cairo_set_source_rgb(context.get(), 0.0, 0.0, 0.0);
  //
  //   cairo_text_extents_t extents {};
  //   cairo_text_extents(context.get(), "hoge", &extents);
  //
  //   cairo_move_to(context.get(), 0, extents.height);
  //
  //   auto size {buffer_.size()};
  //
  //   buffer_.consume(size);
  // }

  auto parse() noexcept(false)
  {
    std::match_results<typename std::basic_string<Char>::const_iterator> results {};

    static const std::basic_regex<Char> apply_raw_input {"^(Ar)([^\\\e]*)(\\\e)?([^]*)$"};

    const std::unique_ptr<cairo_t, decltype(&cairo_destroy)> context {
      cairo_create(meevax::cairo::surface::get()), cairo_destroy
    };

    {
      cairo_select_font_face(context.get(), "Ricty Diminished", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
      cairo_set_font_size(context.get(), 18);

      cairo_text_extents_t extents {};
      cairo_text_extents(context.get(), "hoge", &extents);

      cairo_set_source_rgb(context.get(), 0.0, 0.0, 0.0);
      cairo_paint(context.get());
      cairo_set_source_rgb(context.get(), 1.0, 1.0, 1.0);

      cairo_move_to(context.get(), 0, extents.height);
    }

    for (auto buffer {data()}; !buffer.empty(); )
    {
      if (std::regex_match(buffer, results, apply_raw_input))
      {
        cairo_show_text(context.get(), replace_unprintable(results[2].str()).c_str());

        buffer.erase(
          std::begin(buffer),
          std::begin(buffer) + results[1].length() + results[2].length() + results[3].length()
        );
      }

      else
      {
        // std::cout << "[debug] unimplemented command \""
        //           << replace_unprintable({buffer.front()}) << "\" will be ignored\n";

        buffer.erase(std::begin(buffer), std::begin(buffer) + 1);
      }

      // MEMO
      // システムに対して副作用のある式は最初の構文成立時にのみ実行すること
      // つまり、正規表現末尾の(.*)のサイズがゼロの時のみ実行
    }
  }

  void debug_write()
  {
    const std::unique_ptr<cairo_t, decltype(&cairo_destroy)> context {
      cairo_create(meevax::cairo::surface::get()), cairo_destroy
    };

    {
      cairo_select_font_face(context.get(), "Ricty Diminished", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
      cairo_set_font_size(context.get(), 18);

      cairo_text_extents_t extents {};
      cairo_text_extents(context.get(), "hoge", &extents);

      cairo_set_source_rgb(context.get(), 1.0, 1.0, 1.0);
      cairo_paint(context.get());
      cairo_set_source_rgb(context.get(), 0.0, 0.0, 0.0);

      cairo_move_to(context.get(), 0, extents.height);
    }

    if (!size())
    {
      std::cout << "[debug] debug_write() - buffer is empty\n";
    }
    cairo_show_text(context.get(), replace_unprintable(data()).c_str());
  }

  static auto replace_unprintable(const std::basic_string<Char>& string)
  {
    auto buffer {string};

    using type = std::vector<std::pair<std::basic_string<Char>, std::basic_string<Char>>>;

    for (const auto& pair : type {{"\e", "\\e"}, {"\n", "\\n"}, {"\t", "\\t"}})
    {
      for (auto position {buffer.find(pair.first)}; position != std::basic_string<Char>::npos; )
      {
        buffer.replace(position, pair.first.size(), pair.second);
        position = buffer.find(pair.first, position + pair.second.size());
      }
    }

    return buffer;
  }

  template <typename InputIterator>
  static decltype(auto) replace_unprintable(InputIterator begin, InputIterator end)
  {
    return replace_unprintable({begin, end});
  }

private:
  template <typename InputIterator> // XXX 効率度外視の一時的な処理切り分け
  decltype(auto) write(const std::unique_ptr<cairo_t, decltype(&cairo_destroy)>& context,
                      InputIterator begin, InputIterator end)
  {
    return cairo_show_text(context.get(), replace_unprintable(begin, end).c_str());
  }

public:
  void map() const noexcept
  {
    xcb_map_window(
      meevax::xcb::window::connection.get(),
      meevax::xcb::window::id
    );
  }

  template <typename... Ts>
  decltype(auto) configure(Ts&&... args)
  {
    return xcb_configure_window(
      meevax::xcb::window::connection.get(),
      meevax::xcb::window::id,
      std::forward<Ts>(args)...
    );
  }

  template <typename... Ts>
  decltype(auto) change_attributes(Ts&&... args)
  {
    return xcb_change_window_attributes(
      meevax::xcb::window::connection.get(),
      meevax::xcb::window::id,
      std::forward<Ts>(args)...
    );
  }
};


template <typename T, typename Functor,
          typename = typename std::enable_if<meevax::has_function_call_operator<Functor, T&>::value>::type>
inline decltype(auto) operator<<(T& lhs, Functor&& rhs)
{
  return rhs(lhs);
}


template <typename Char>
decltype(auto) operator<<(meevax::basic_vstream<Char>& ostream, Char input)
{
  return ostream << std::basic_string<Char> {input};
}


template <typename Char>
decltype(auto) operator<<(meevax::basic_vstream<Char>& ostream, std::basic_string<Char> input)
{
  ostream.prepare(input.size());

  std::copy(std::begin(input), std::end(input), std::ostream_iterator<Char> {ostream});
  ostream.commit(input.size());

  ostream.debug_write();

  return ostream;
}


template <typename Char>
decltype(auto) operator<<(meevax::basic_vstream<Char>& ostream, meevax::basic_vstream<Char>& istream)
{
  ostream.prepare(istream.size());

  std::copy(std::istream_iterator<Char> {istream}, std::istream_iterator<Char> {}, std::ostream_iterator<Char> {ostream});
  ostream.commit(istream.size());

  istream.consume(istream.size());
  istream.clear();

  ostream.debug_write();

  return ostream;
}


template <typename Char>
decltype(auto) operator<<(std::basic_ostream<Char>& ostream, meevax::basic_vstream<Char>& istream)
{
  decltype(istream.data()) buffer {};
  istream >> buffer;

  ostream << meevax::basic_vstream<Char>::replace_unprintable(buffer); // TODO replace to free function

  istream.clear();

  return ostream;
}


} // namespace meevax


#endif

