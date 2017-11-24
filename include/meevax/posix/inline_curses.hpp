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

  using typename SequenceContainer<std::basic_string<char_type>>::difference_type;
  using typename SequenceContainer<std::basic_string<char_type>>::size_type;

  size_type window_row;
  size_type window_column;

private:
  std::basic_istream<char_type>& istream_; // TODO std::stack に詰めて出力多重化に備えること
  std::basic_ostream<char_type>& ostream_;

  boost::value_initialized<size_type> window_upper_left_row_;
  boost::value_initialized<size_type> window_upper_left_column_;

  boost::value_initialized<difference_type> relative_cursor_row_; // TODO イテレータ化
  boost::value_initialized<difference_type> relative_cursor_column_;

public:
  explicit inline_curses(std::basic_istream<char_type>& istream,
                         std::basic_ostream<char_type>& ostream) noexcept(false)
    : SequenceContainer<std::basic_string<CharType>> {""},
      istream_ {istream},
      ostream_ {ostream}
  {
    ostream_ << "\e[0;38;5;059m" << "  0 ready, you have control." << "\n~\e[0m" << std::flush;
  }

  template <typename T>
  void update_window_size(T ws_row, T ws_col) noexcept
  {
    window_row    = static_cast<size_type>(ws_row);
    window_column = static_cast<size_type>(ws_col);
  }

  void read() noexcept(false)
  {
    std::basic_string<char_type> buffer {istream_.get()};

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

      // +1 はゼロ始まりのインデックスをサイズにするための処理
      // -1 はウィンドウサイズから最下行のステータスライン文の行を除く処理
      if (relative_cursor_row_ + 1 < window_row - 1)
      {
        ++relative_cursor_row_;
        ostream_ << "\n"; // XXX UGLY CODE (FOR REWIND)
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

  // TODO window_row, window_column に値がセットされているかの確認（初期化の追加含む）
  //      セットされていない場合、出力先を端末ではない（ファイルストリームとか）とみなして出力
  void write() const
  {
    ostream_ << "\e[?7l" << "\e[?25l" << std::flush;

    auto iter {std::begin(*this)}; std::advance(iter, window_upper_left_row_);

    ostream_ << "\e[" << std::min((*this).size(), window_row) << "A";

    for (const auto first {iter};
         iter != std::end(*this) && std::distance(first, iter) < window_row - 1;
         ++iter)
    {
      ostream_ << "\r\e[K";

      [&](auto number) -> void
      {
        ostream_ << "\e[0;38;5;059m"
                 << std::setw(std::max(std::to_string((*this).size()).size(), size_type {2}))
                 << std::right << number << "\e[0m ";
      }(std::distance(std::begin(*this), iter));

      ostream_ << *iter << "\n";
    }

    ostream_ << "\r" << std::setw(window_column) << std::right << status().str() << "\e[D";

    ostream_ << "\e[?7h" << "\e[?25h" << std::flush;
  }

private:
  auto status() const
  {
    std::basic_stringstream<char_type> sstream {};

    sstream << "window position: [" << window_upper_left_row_ << ", "
                                    << window_upper_left_column_ << "], ";

    sstream << "cursor: ["
            << window_upper_left_row_ + relative_cursor_row_
            << ", "
            << window_upper_left_column_ + relative_cursor_column_
            << "], size: "
            << std::accumulate(std::begin(*this), std::end(*this), std::basic_string<char_type> {}).size() + (*this).size()
            << " ";

    return sstream;
  }
};


#if __cplusplus < 201703L
} // namespace posix
} // namespace meevax
#else
} // namespace meevax::posix
#endif


#endif

