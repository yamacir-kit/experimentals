#ifndef INCLUDED_MEEVAX_VSTREAM_BASIC_VSTREAM_HPP
#define INCLUDED_MEEVAX_VSTREAM_BASIC_VSTREAM_HPP


#include <algorithm>
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

#include <meevax/algorithm/regex_split_include_delimiter.hpp>
#include <meevax/cairo/surface.hpp>
#include <meevax/string/replace_unprintable.hpp>
#include <meevax/type_traits/has_operator.hpp>

#ifndef NDEBUG
#include <meevax/string/runtime_typename.hpp>
#endif


namespace meevax {


template <typename Char>
class basic_vstream
  : public meevax::cairo::surface,
    public std::basic_iostream<Char>
{
public:
  using char_type = typename std::basic_iostream<Char>::char_type;

  boost::asio::basic_streambuf<std::allocator<char_type>> buffer;

public:
  basic_vstream() = delete;

  template <typename... Ts>
  explicit basic_vstream(Ts&&... args)
    : meevax::cairo::surface {std::forward<Ts>(args)...},
      std::basic_iostream<char_type> {&buffer}
  {}

public:
  // [[deprecated]] auto output() // write to window surface
  // {
  //   std::vector<std::basic_string<Char>> buffer {};
  //
  //   static const std::vector<std::basic_regex<Char>> delimiters {
  //     std::basic_regex<Char> {"\\\e\\[(\\d*;?)+(.)"}, // escape sequence
  //     std::basic_regex<Char> {"\n"}
  //   };
  //
  //   meevax::algorithm::regex_split_include_delimiter(buffer, data(), delimiters);
  //
  //   buffer.erase(
  //     std::remove_if(std::begin(buffer), std::end(buffer), [](auto s) { return s.empty(); }),
  //     std::end(buffer)
  //   );
  //
  //   std::match_results<typename std::basic_string<Char>::const_iterator> results {};
  //
  //   static const std::basic_regex<Char>  crlf_regex {"^\n$"};
  //   static const std::basic_regex<Char> color_regex {"^\\\e\\[(\\d+);(\\d+);(\\d+);(\\d+);(\\d+)m$"};
  //
  //   const std::unique_ptr<cairo_t, decltype(&cairo_destroy)> cairo {
  //     cairo_create(meevax::cairo::surface::get()), cairo_destroy
  //   };
  //
  //   cairo_select_font_face(cairo.get(), "Ricty Diminished", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
  //   cairo_set_font_size(cairo.get(), 18);
  //
  //   cairo_text_extents_t extents {};
  //   cairo_text_extents(cairo.get(), "hoge", &extents);
  //
  //   cairo_set_source_rgb(cairo.get(), 1.0, 1.0, 1.0);
  //   cairo_paint(cairo.get());
  //   cairo_set_source_rgb(cairo.get(), 0.0, 0.0, 0.0);
  //
  //   cairo_move_to(cairo.get(), 0, extents.height);
  //
  //   for (const auto& s : buffer)
  //   {
  //     if (std::regex_match(s, results, crlf_regex))
  //     {
  //       double x, y;
  //       cairo_get_current_point(cairo.get(), &x, &y);
  //       boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian> point {x, y};
  //
  //       static constexpr double line_margin {3};
  //       cairo_move_to(cairo.get(), 0, boost::geometry::get<1>(point) + extents.height + line_margin);
  //     }
  //
  //     else if (std::regex_match(s, results, color_regex)) // XXX UGLY CODE
  //     {
  //       if (std::stoi(results[1]) == 38 && std::stoi(results[2]) == 2)
  //       {
  //         cairo_set_source_rgb(
  //           cairo.get(),
  //           std::stod(results[3]) / std::numeric_limits<std::int8_t>::max(),
  //           std::stod(results[4]) / std::numeric_limits<std::int8_t>::max(),
  //           std::stod(results[5]) / std::numeric_limits<std::int8_t>::max()
  //         );
  //       }
  //       else if (std::stoi(results[1]) == 48 && std::stoi(results[2]) == 2)
  //       {
  //         cairo_set_source_rgb(
  //           cairo.get(),
  //           std::stod(results[3]) / std::numeric_limits<std::int8_t>::max(),
  //           std::stod(results[4]) / std::numeric_limits<std::int8_t>::max(),
  //           std::stod(results[5]) / std::numeric_limits<std::int8_t>::max()
  //         );
  //
  //         cairo_paint(cairo.get());
  //       }
  //       else
  //       {
  //         std::basic_string<Char> invalid {results[0]};
  //         invalid.replace(std::begin(invalid), std::begin(invalid) + 1, "\\e");
  //
  //         cairo_show_text(cairo.get(), invalid.c_str());
  //       }
  //     }
  //
  //     else
  //     {
  //       write(cairo, std::begin(s), std::end(s));
  //     }
  //   }
  // }

  [[deprecated]] auto parse() noexcept(false)
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

    for (auto data {buffer.data()}; !data.empty(); )
    {
      if (std::regex_match(data, results, apply_raw_input))
      {
        cairo_show_text(context.get(), replace_unprintable(results[2].str()).c_str());

        data.erase(
          std::begin(data),
          std::begin(data) + results[1].length() + results[2].length() + results[3].length()
        );
      }

      else
      {
        // std::cout << "[debug] unimplemented command \""
        //           << replace_unprintable({data.front()}) << "\" will be ignored\n";

        data.erase(std::begin(data), std::begin(data) + 1);
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

    const std::basic_string<Char> string {
      boost::asio::buffers_begin(buffer.data()),
      boost::asio::buffers_end(buffer.data())
    };

    cairo_show_text(context.get(), string.c_str());
    cairo_surface_flush(meevax::cairo::surface::get());
  }

  decltype(auto) copy_to(meevax::basic_vstream<char_type>& ostream) const
  {
    auto size {boost::asio::buffer_copy(
      ostream.buffer.prepare(buffer.size()),
      buffer.data()
    )};

    ostream.buffer.commit(size);
    ostream.debug_write(); // TODO commit に描画処理を統合

    return size;
  }

  decltype(auto) copy_to(std::basic_ostream<char_type>& ostream) const
  {
    std::copy(
      boost::asio::buffers_begin(buffer.data()),
      boost::asio::buffers_end(buffer.data()),
      std::ostreambuf_iterator<char_type> {ostream}
    );

    return std::distance(
      boost::asio::buffers_begin(buffer.data()),
      boost::asio::buffers_end(buffer.data())
    );
  }
};


template <typename T, typename Functor,
          typename = typename std::enable_if<
                                std::is_base_of<
                                  std::basic_ostream<typename std::remove_reference<T>::type::char_type>,
                                  typename std::remove_reference<T>::type
                                >::value
                              >::type,
          typename = typename std::enable_if<
                                meevax::has_function_call_operator<Functor, T>::value
                              >::type
         >
constexpr decltype(auto) operator<<(T&& lhs, Functor&& rhs)
{
  return rhs(std::forward<T>(lhs));
}


template <typename T, typename U,
          typename = typename std::enable_if<
                                std::is_base_of<
                                  std::basic_ostream<typename std::remove_reference<T>::type::char_type>,
                                  typename std::remove_reference<T>::type
                                >::value
                              >::type,
          typename = typename std::enable_if<
                                std::is_base_of<
                                  std::basic_ostream<typename std::remove_reference<U>::type::char_type>,
                                  typename std::remove_reference<U>::type
                                >::value
                              >::type
         >
#ifdef NDEBUG
constexpr
#endif
decltype(auto) operator,(T&& lhs, U&& rhs)
{
#ifndef NDEBUG
#ifdef DEBUG_MEEVAX_BASIC_VSTREAM_OPREATORS
  std::cerr << "[debug] meevax::operator, - typename T = "
            << meevax::string::runtime_typename<char>(lhs) << "\n"
            << "                            typename U = "
            << meevax::string::runtime_typename<char>(rhs) << std::endl;
#endif
#endif
  return std::forward_as_tuple(
    std::forward<T>(lhs), std::forward<U>(rhs)
  );
}


template <typename... Ts, typename T,
          typename = typename std::enable_if<
                                std::is_base_of<
                                  std::basic_ostream<typename std::remove_reference<T>::type::char_type>,
                                  typename std::remove_reference<T>::type
                                >::value
                              >::type
         >
#ifdef NDEBUG
constexpr
#endif
decltype(auto) operator,(std::tuple<Ts...>&& lhs, T&& rhs)
{
#ifndef NDEBUG
#ifdef DEBUG_MEEVAX_BASIC_VSTREAM_OPREATORS
  std::cerr << "[debug] meevax::operator, - std::tuple<Ts...> = "
            << meevax::string::runtime_typename<char>(lhs) << "\n"
            << "                            typename U = "
            << meevax::string::runtime_typename<char>(rhs) << std::endl;
#endif
#endif
  return std::forward_as_tuple(
    std::forward<Ts>(std::get<Ts>(lhs))..., std::forward<T>(rhs)
  );
}


template <typename T, typename Char,
          typename = typename std::enable_if<
                                std::is_base_of<
                                  std::basic_ostream<typename std::remove_reference<T>::type::char_type>,
                                  typename std::remove_reference<T>::type
                                >::value
                              >::type
         >
#ifdef NDEBUG
constexpr
#endif
decltype(auto) operator<<(T&& ostream, meevax::basic_vstream<Char>& istream)
{
#ifndef NDEBUG
#ifdef DEBUG_MEEVAX_BASIC_VSTREAM_OPREATORS
  std::cerr << "[debug] meevax::operator<< - typename T = "
            << meevax::string::runtime_typename<char>(ostream) << "\n"
            << "        serial data transfer from "
            << meevax::string::runtime_typename<char>(istream) << std::endl;
#endif
#endif
  return std::tuple<T> {std::forward<T>(ostream)} << istream;
}


template <typename T, typename U, typename... Ts, typename Char>
decltype(auto) operator<<(std::tuple<T, U, Ts...>&& ostreams, meevax::basic_vstream<Char>& istream)
{
#ifndef NDEBUG
#ifdef DEBUG_MEEVAX_BASIC_VSTREAM_OPREATORS
  std::cerr << "[debug] std::tuple<T, U, Ts...> = "
            << meevax::string::runtime_typename<char>(ostreams) << std::endl;
  std::cerr << "[debug] copying " << std::flush;
#endif
#endif
  istream.copy_to(std::forward<T>(std::get<T>(ostreams)));

  return std::forward_as_tuple(
    std::forward<U>(std::get<U>(ostreams)), std::forward<Ts>(std::get<Ts>(ostreams))...
  ) << istream;
}


template <typename T, typename Char>
decltype(auto) operator<<(std::tuple<T>&& ostream, meevax::basic_vstream<Char>& istream)
{
#ifndef NDEBUG
#ifdef DEBUG_MEEVAX_BASIC_VSTREAM_OPREATORS
  std::cerr << "[debug] std::tuple<T> = "
            << meevax::string::runtime_typename<char>(ostream) << std::endl;
  std::cout << "[debug] transferring ";
#endif
#endif
  istream.buffer.consume(
    istream.copy_to(std::forward<T>(std::get<T>(ostream)))
  );

  return std::get<T>(ostream);
}


} // namespace meevax


#endif

