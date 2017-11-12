#ifndef INCLUDED_MEEVAX_ANSI_ESCAPE_SEQUENCE_CURSOR_HPP
#define INCLUDED_MEEVAX_ANSI_ESCAPE_SEQUENCE_CURSOR_HPP


#include <iostream>
#include <string>

#include <meevax/string/static_concat.hpp>


namespace meevax::ansi_escape_sequence::cursor {


#ifdef _WIN32
static constexpr auto newline {meevax::string::static_concat("\r\n")};
#elif defined macintosh
static constexpr auto newline {meevax::string::static_concat("\r")};
#else
static constexpr auto newline {meevax::string::static_concat("\n")};
#endif


class counted_newline_
{
  static signed int count_;

public:
  explicit counted_newline_() = default;

  auto countup() noexcept
  {
    return (++count_, newline);
  }

  auto restore() noexcept // XXX carriage return はコイツの仕事ではない
  {
    const std::string cursor_up {
      std::string {"\e["} + std::to_string(count_) + std::string {"A"}
    };

    count_ = 0;
    return cursor_up + "\r\e[K";
  }
} counted_newline;


signed int counted_newline_::count_ {0};


} // namespace meevax::ansi_escape_sequence::cursor


namespace {


using namespace meevax::ansi_escape_sequence::cursor;


template <typename Char>
inline decltype(auto) operator<<(std::basic_ostream<Char>& lhs, counted_newline_& rhs)
{
  return lhs << rhs.countup();
}


} // namespace


#endif

