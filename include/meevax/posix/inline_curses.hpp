#ifndef INCLUDED_MEEVAX_POSIX_INLINE_CURSES_HPP
#define INCLUDED_MEEVAX_POSIX_INLINE_CURSES_HPP


#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

#include <boost/utility/value_init.hpp>

#include <unistd.h>

#include <meevax/concepts/is_standard_container.hpp>


#if __cplusplus < 201703L
namespace meevax {
namespace posix {
#else
namespace meevax::posix {
#endif


template <typename CharType, template <typename...> typename SequenceContainer = std::list,
          typename = typename std::enable_if<
                                meevax::concepts::is_standard_container<
                                  SequenceContainer<std::basic_string<CharType>>
                                >::value
                              >::type>
class inline_curses
  : public SequenceContainer<std::basic_string<CharType>>
{
public:
  using char_type = CharType;

  template <typename T>
  using container_type = SequenceContainer<T>;

  using typename container_type<std::basic_string<char_type>>::difference_type;
  using typename container_type<std::basic_string<char_type>>::size_type;

private:
  std::basic_istream<char_type>& istream_; // TODO std::stack に詰めて出力多重化に備えること
  std::basic_ostream<char_type>& ostream_;

  const ::winsize& winsize_;

  boost::value_initialized<size_type> window_upper_left_row_;
  boost::value_initialized<size_type> window_upper_left_column_;

  typename container_type<std::basic_string<char_type>>::iterator cursor_row_;
  typename std::basic_string<char_type>::iterator cursor_column_;

public:
  explicit inline_curses(std::basic_istream<char_type>& istream,
                         std::basic_ostream<char_type>& ostream,
                         ::winsize& winsize)
    : container_type<std::basic_string<char_type>> {""},
      istream_ {istream},
      ostream_ {ostream},
      winsize_ {winsize},
      cursor_row_ {std::begin(*this)},
      cursor_column_ {std::begin(*cursor_row_)}
  {
    ostream_ << line_number(std::begin(*this)) << "\e[0;38;5;059mready, you have control." << "\n\e[0m" << std::flush;
    ostream_ << std::right << std::setw(winsize_.ws_col) << status().str() << "\e[D";
  }

  void read() noexcept(false)
  {
    std::basic_string<char_type> buffer {static_cast<char_type>(istream_.get())};

    switch (buffer.back()) // TODO regex 構築がゲロ重なので static const にすること
    {
    case 0x1B:
      while (!std::regex_match(buffer, std::regex {"^\\\e\\[(\\d*;?)+(.)$"}))
      {
        buffer.push_back(istream_.get());
      }
      break;

    case '\n':
      (*this).emplace_back("");

      if (std::distance(std::begin(*this), ++cursor_row_) + wc_lines(status().str()) < winsize_.ws_row)
      {
        ostream_ << "\n";
      }
      else
      {
        ++window_upper_left_row_;
      }
      break;

    default:
      assert(std::isprint(buffer.back()));
      (*this).back() += buffer;
    }
  }

  void write()
  {
    ostream_ << "\e[?7l" << "\e[?25l" << std::flush;

    auto iter {std::begin(*this)}; std::advance(iter, window_upper_left_row_);

    ostream_ << "\e["
             << std::min(
                    (*this).size(),
                    static_cast<size_type>(winsize_.ws_row)
                  )
             << "A";

    for (const auto first {iter};
         iter != std::end(*this)
           && std::distance(first, iter) + 1 < winsize_.ws_row;
         ++iter)
    {
      ostream_ << "\r\e[K";

      // ostream_ << "\e[0;38;5;059m"
      //          << std::setw(std::max(std::to_string((*this).size()).size(), size_type {2}))
      //          << std::right << std::distance(std::begin(*this), iter) << "\e[0m ";
      ostream_ << line_number(iter);

      ostream_ << *iter << "\n";
    }

    ostream_ << std::right << std::setw(winsize_.ws_col) << status().str() << "\e[D";

    ostream_ << "\e[?7h" << "\e[?25h" << std::flush;
  }

private:
  auto status()
  {
    std::basic_stringstream<char_type> sstream {};

    sstream << "window position: [" << window_upper_left_row_ << ", "
                                    << window_upper_left_column_ << "], ";

    sstream << "cursor: ["
            << std::distance(std::begin(*this), cursor_row_)
            << ", "
            << std::distance(std::begin(*cursor_row_), cursor_column_)
            << "], size: "
            << std::accumulate(std::begin(*this), std::end(*this), std::basic_string<char_type> {}).size() + (*this).size()
            << " byte ";

    return sstream;
  }

  auto line_number(const decltype(cursor_row_)& iter)
  {
    std::basic_stringstream<char_type> buffer {};

    const std::basic_string<char_type> highlight {
      std::distance(std::begin(*this), iter) != std::distance(std::begin(*this), cursor_row_) ?  "\e[0;38;5;059m" : "\e[0;38;5;221m"
    };

    std::copy(
      std::begin(highlight), std::end(highlight), std::ostreambuf_iterator<char_type> {buffer}
    );

    buffer << std::setw(std::max(std::to_string((*this).size()).size(), size_type {2}))
           << std::right << std::distance(std::begin(*this), iter)
           << "\e[0m ";

    return buffer.str();
  }

  auto wc_lines(const std::basic_string<char_type>& string) const
  {
    return std::count(std::begin(string), std::end(string), '\n') + 1;
  }
};


#if __cplusplus < 201703L
} // namespace posix
} // namespace meevax
#else
} // namespace meevax::posix
#endif


#endif

