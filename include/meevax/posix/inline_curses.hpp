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
#include <system_error>
#include <utility>

#include <boost/utility/value_init.hpp>

#include <sys/ioctl.h>
#include <unistd.h>

#include <meevax/posix/read.hpp>


#if __cplusplus < 201703L
namespace meevax {
namespace posix {
#else
namespace meevax::posix {
#endif


template <typename CharType, template <typename...> typename SequenceContainer = std::list>
class inline_curses
  : public ::winsize,
    public SequenceContainer<std::basic_string<CharType>>
{
public:
  using char_type = CharType;

  using typename SequenceContainer<std::basic_string<char_type>>::difference_type;
  using typename SequenceContainer<std::basic_string<char_type>>::size_type;

private:
  const int fd_;

  std::basic_ostream<char_type>& ostream_;

  boost::value_initialized<size_type> window_upper_left_row_;
  boost::value_initialized<size_type> window_upper_left_column_;

  boost::value_initialized<difference_type> relative_cursor_row_; // TODO イテレータ化
  boost::value_initialized<difference_type> relative_cursor_column_;

public:
  explicit inline_curses(int fd, std::basic_ostream<char_type>& ostream) noexcept(false)
    : fd_ {::isatty(fd) ? fd : throw std::system_error {errno, std::system_category()}},
      ostream_ {ostream}
  {
    ::ioctl(fd_, TIOCGWINSZ, this);
  }

  void read() noexcept(false)
  {
    std::basic_string<char_type> buffer {meevax::posix::read<char_type>(fd_)};

    switch (buffer.back()) // TODO regex 構築がゲロ重なので static const にすること
    {
    case 0x1B:
      while (!std::regex_match(buffer, std::regex {"^\\\e\\[(\\d*;?)+(.)$"}))
      {
        buffer.push_back(meevax::posix::read<char_type>(fd_));
      }
      break;

    case '\n':
      (*this).emplace_back("");

      // +1 はゼロ始まりのインデックスをサイズにするための処理
      // -1 はウィンドウサイズから最下行のステータスライン文の行を除く処理
      if (relative_cursor_row_ + 1 < (*this).ws_row - 1)
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

  void write()
  {
    ostream_ << "\e[?7l" << std::flush;

    if (!(*this).empty())
    {
      auto iter {std::begin(*this)}; std::advance(iter, window_upper_left_row_);

      rewind();

      for (const auto first {iter};
           iter != std::end(*this) && std::distance(first, iter) < (*this).ws_row - 1;
           ++iter)
      {
        ostream_ << "\r\e[K";

        [&](auto number) -> void
        {
          ostream_ << "\e[0;38;5;059m"
                  << std::setw(std::to_string((*this).size()).size()) << std::right
                  << number << "\e[0m ";
        }(std::distance(std::begin(*this), iter));

        ostream_ << *iter << "\n";
      }
    }
    else
    {
      (*this).emplace_back("");
      ostream_ << "\e[0;38;5;059m" << "ready, you have control." << "\e[0m\n";
    }

    std::basic_stringstream<char_type> sstream {};
    {
      sstream << "window position: [" << window_upper_left_row_ << ", "
                                      << window_upper_left_column_ << "], ";

      sstream << "cursor: ["
              << window_upper_left_row_ + relative_cursor_row_
              << ", "
              << window_upper_left_column_ + relative_cursor_column_
              << "], size: "
              << std::accumulate(std::begin(*this), std::end(*this), std::basic_string<char_type> {}).size() + (*this).size()
              << " ";
    }

    ostream_ << "\r" << std::setw((*this).ws_col) << std::right << sstream.str() << "\e[D";

    ostream_ << "\e[?7h" << std::flush;
  }

private:
  void rewind()
  {
    ostream_ << "\e[" << std::min((*this).size(), static_cast<size_type>((*this).ws_row)) << "A";
  }
};


#if __cplusplus < 201703L
} // namespace posix
} // namespace meevax
#else
} // namespace meevax::posix
#endif


#endif

