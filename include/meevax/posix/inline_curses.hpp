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
#include <boost/lexical_cast.hpp>

#include <unistd.h>

#include <meevax/concepts/is_standard_container.hpp>
#include <meevax/string/replace_unprintable.hpp>


#if __cplusplus < 201703L
namespace meevax {
namespace posix {
#else
namespace meevax::posix {
#endif


// TODO
// データを書き換えてはならない処理へ const_iterator でアクセスするように変更すること


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

  using typename container_type<std::basic_string<char_type>>::size_type;

private:
  std::basic_istream<char_type>& istream_; // TODO std::stack に詰めて出力多重化に備えること
  std::basic_ostream<char_type>& ostream_;

  const winsize& winsize_;

  boost::value_initialized<size_type> scroll_row_, scroll_column_;

  typename container_type<std::basic_string<char_type>>::iterator cursor_row_;
  typename std::basic_string<char_type>::iterator cursor_column_;

public:
  std::basic_string<char_type> dealt_with, buffer;

public:
  explicit inline_curses(std::basic_istream<char_type>& istream,
                         std::basic_ostream<char_type>& ostream,
                         winsize& winsize)
    : container_type<std::basic_string<char_type>> {""},
      istream_ {istream},
      ostream_ {ostream},
      winsize_ {winsize},
      cursor_row_ {std::begin(*this)},
      cursor_column_ {std::end(*cursor_row_)}
  {
    write();
  }

  // 内部イテレータへ入力に対して意味的に正しく一対一に対応する動作を行う
  void read() noexcept(false)
  {
    buffer = istream_.get();

    switch (buffer.back())
    {
    case 0x1B: // TODO regex 構築がゲロ重なので static const にすること
      {
        while (!std::regex_match(buffer, std::regex {"^\\\e\\[(\\d*;?)+(.~)$"}))
        {
          buffer.push_back(istream_.get());

          // TODO この対処は逃げ
          if (buffer[0] == 0x1B && buffer[1] == 0x1B)
          {
            break;
          }
        }

        if (buffer == "\e[A" && cursor_row_ != std::begin(*this))
        {
          const auto distance {std::distance(std::begin(*cursor_row_--), cursor_column_)};
          if ((*cursor_row_).size() < distance)
          {
            cursor_column_ = std::end(*cursor_row_);
          }
          else
          {
            cursor_column_ = std::begin(*cursor_row_) + distance;
          }

          if (std::distance(std::begin(*this), cursor_row_) < scroll_row_)
          {
            --scroll_row_;
          }

          break;
        }

        // XXX 条件が (*this).back() にイテレータが当たってる時点で弾かないといけない
        if (buffer == "\e[B" && std::distance(std::begin(*this), cursor_row_) < (*this).size() - 1)
        {
          const auto distance {std::distance(std::begin(*cursor_row_++), cursor_column_)};
          if ((*cursor_row_).size() < distance)
          {
            cursor_column_ = std::end(*cursor_row_);
          }
          else
          {
            cursor_column_ = std::begin(*cursor_row_) + distance;
          }

          if (winsize_.ws_row + scroll_row_ - wc_lines(status_line())
                <= std::distance(std::begin(*this), cursor_row_))
          {
            ++scroll_row_;
          }

          break;
        }

        if (buffer == "\e[C" && cursor_column_ != std::end(*cursor_row_))
        {
          ++cursor_column_;
          break;
        }

        if (buffer == "\e[D" && cursor_column_ != std::begin(*cursor_row_))
        {
          --cursor_column_;
          break;
        }

        dealt_with = "unimplemented sequences: ";
        throw std::runtime_error {dealt_with + meevax::string::replace_unprintable(buffer)};
      }
      break; // XXX 将来的に fall through させて default に飛ばすのもアリか

    case '\n':
      {
        (*this).emplace(cursor_row_, std::begin(*cursor_row_), cursor_column_);
        cursor_column_ = (*cursor_row_).erase(std::begin(*cursor_row_), cursor_column_);

        if (std::distance(std::begin(*this), cursor_row_) + wc_lines(status_line()) < winsize_.ws_row)
        {
          ostream_ << "\n";
        }
        else
        {
          ++scroll_row_;
        }
      }
      break;

    case 0x7F:
      {
        if (cursor_column_ != std::begin(*cursor_row_))
        {
          buffer = *--cursor_column_;
          cursor_column_ = (*cursor_row_).erase(cursor_column_);

          dealt_with = "removed: ";
        }
        else
        {
          dealt_with = "unimplemented: ";
          buffer = "0x7F";
        }
      }
      break;

    default:
      if (std::isprint(buffer.back()))
      {
        ++(cursor_column_ = (*cursor_row_).insert(cursor_column_, std::begin(buffer), std::end(buffer)));

        dealt_with = "printable: ";
        buffer = std::to_string(static_cast<int>(buffer.back()));
      }
      else
      {
        dealt_with = "unimplemented: ";
        buffer = std::to_string(static_cast<int>(buffer.back()));
      }
      break;
    }
  }

  void write()
  {
    auto scrolled {std::begin(*this)}; std::advance(scrolled, scroll_row_);
    static auto cursor_row {scrolled};

    // UNWRAP LINE AND INVISIBLE CURSOR
    ostream_ << "\e[?7l" << "\e[?25l" << std::flush;

    // REWIND
    for (; cursor_row != scrolled; --cursor_row)
    {
      ostream_ << "\e[A";
    }

    while (cursor_row != std::end(*this)
           && std::distance(scrolled, cursor_row) + wc_lines(status_line()) < winsize_.ws_row)
    {
      ostream_ << "\r\e[K" << line_number(cursor_row) << *(cursor_row++) << "\n";
    }

    // WRITE STATUS LINE
    ostream_ << "\r\e[K" << std::right << std::setw(winsize_.ws_col) << status_line();
    ostream_ << "\e[D\r-- INSERT -- ";

    // WRAP LINE AND VISIBLE CURSOR
    ostream_ << "\e[?7h" << "\e[?25h" << std::flush;

    // MOVE CURSOR ROW TO MATCH "cursor_row_" VISUALLY
    ostream_ << "\e[" << wc_lines(status_line()) << "A";
    while (--cursor_row != cursor_row_)
    {
      ostream_ << "\e[A";
    }

    // MOVE CURSOR COLUMN TO MATCH "cursor_column_" VISUALLY
    ostream_ << "\r\e["
             << std::distance(std::begin(*cursor_row_), cursor_column_)
                  + std::max(std::to_string((*this).size()).size(), size_type {2})
                  + 1
             << "C";
  }

private:
  auto status_line() // XXX 綺麗じゃないのが気になるけど、まあここはこういう部位だろう
  {
    std::basic_stringstream<char_type> sstream {};

    if (!buffer.empty())
    {
      sstream << dealt_with << meevax::string::replace_unprintable(buffer) << ", ";
    }

    // sstream << "window: [" << winsize_.ws_row << ", " << winsize_.ws_col << "], ";

    // sstream << "scroll: [" << scroll_row_ << ", " << scroll_column_ << "], ";

    sstream << "cursor: ["
            << std::distance(std::begin(*this), cursor_row_) << ", "
            << std::distance(std::begin(*cursor_row_), cursor_column_) << "], ";

    // XXX ここクソコード
    sstream << "size: "
            << std::accumulate(std::begin(*this), std::end(*this), std::basic_string<char_type> {}).size() + (*this).size()
            << " byte ";

    return sstream.str();
  }

  auto line_number(const decltype(cursor_row_)& iter)
  {
    std::basic_stringstream<char_type> sstream {};

    const std::basic_string<char_type> highlight {
      std::distance(std::begin(*this), iter) != std::distance(std::begin(*this), cursor_row_) ?  "\e[0;38;5;059m" : "\e[0;38;5;221m"
    };

    std::copy(
      std::begin(highlight), std::end(highlight), std::ostreambuf_iterator<char_type> {sstream}
    );

    sstream << std::setw(std::max(std::to_string((*this).size()).size(), size_type {2}))
            << std::right << std::distance(std::begin(*this), iter)
            << "\e[0m ";

    return sstream.str();
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

