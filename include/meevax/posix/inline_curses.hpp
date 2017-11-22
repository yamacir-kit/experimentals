#ifndef INCLUDED_MEEVAX_POSIX_INLINE_CURSES_HPP
#define INCLUDED_MEEVAX_POSIX_INLINE_CURSES_HPP


#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <regex>
#include <string>
#include <system_error>
#include <utility>

#include <sys/ioctl.h>
#include <unistd.h>

#include <meevax/posix/read.hpp>


#if __cplusplus < 201703L
namespace meevax {
namespace posix {
#else
namespace meevax::posix {
#endif


template <typename RowIterator, typename ColIterator>
class cursor
  : public std::pair<RowIterator, ColIterator>
{
public:
  using row_iterator = RowIterator;
  using col_iterator = ColIterator;

  row_iterator& row_iter;
  col_iterator& col_iter;

public:
  template <typename... Ts>
  explicit cursor(Ts&&... args)
    : std::pair<row_iterator, col_iterator> {std::forward<Ts>(args)...},
      row_iter {(*this).first},
      col_iter {(*this).second}
  {}
};


class area
  : public ::winsize
{
  const int fd_;

  decltype(::winsize::ws_row) row_;

public:
  // [[deprecated]] const decltype(::winsize::ws_row)& max_row;
  // [[deprecated]] const decltype(::winsize::ws_col)& max_col;

public:
  explicit area(int fd) noexcept(false)
    : fd_ {::isatty(fd) ? fd : throw std::system_error {errno, std::system_category()}},
      row_ {1}
      // max_row {(*this).max_row},
      // max_col {(*this).max_col}
  {
    ::ioctl(fd_, TIOCGWINSZ, this); // TODO SET SIGNAL HANDLER
  }

  const auto& increment_row() noexcept
  {
    return row_ = std::min(static_cast<decltype(::winsize::ws_row)>(row_ + 1), ws_row);
  }

  const auto& assign_row(decltype(row_) row)
  {
    return row_ = std::min(std::max(row_, row), ws_row);
  }

  const auto& row() const noexcept
  {
    return row_;
  }
};


template <typename Char>
class inline_curses_
  : public std::list<std::basic_string<Char>>
{
public:
  using char_type = Char;

private:
  const int fd_;

  meevax::posix::cursor<
    typename std::list<std::basic_string<char_type>>::iterator,
    typename std::basic_string<char_type>::iterator
  > cursor_;

  meevax::posix::area area_;

public:
  explicit inline_curses_(int fd) noexcept(false)
    : std::list<std::basic_string<char_type>> {""},
      fd_ {::isatty(fd) ? fd : throw std::system_error {errno, std::system_category()}},
      cursor_ {std::begin(*this), std::begin(*std::begin(*this))},
      area_ {fd}
  {}

  void rewind(std::basic_ostream<char_type>& ostream) const
  {
    if (area_.row() != area_.ws_row)
    {
      for (auto row {1}; row < area_.row(); ++row)
      {
        ostream << "\e[A";
      }
    }
    else
    {
      ostream << "\e[0;0H";
    }
  }

  void draw(std::basic_ostream<char_type>& ostream)
  {
    rewind(ostream);

    ostream << "\e[?7l" << std::flush;

    // ostream << "\r\e[K\e[31mcursor iterator row: "
    //         << std::distance(            std::begin(*this),  cursor_.row_iter) << ", col: "
    //         << std::distance(std::begin(*std::begin(*this)), cursor_.col_iter) << ", size: "
    //         << area_.row() << "/" << area_.ws_row << "\e[0m\n";

    for (auto iter {std::begin(*this)}; iter != std::end(*this); ++iter)
    {
      ostream << "\r\e[K\e[33m" << std::setw(std::to_string((*this).size()).size())
              << std::right << std::distance(std::begin(*this), iter) << "\e[0m ";

      ostream << *iter
              << (std::distance(&(*this).back(), &*iter) != 0 ? "\n" : "");
    }

    area_.assign_row(this->size()); // XXX +1 はデバッグ用のイテレータ位置表示のため

    ostream << "\e[?7h" << std::flush;
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
      ++(cursor_.row_iter);
      cursor_.col_iter = std::begin(*std::begin(*this)); // TODO インデントの考慮

      break;

    default:
      assert(std::isprint(buffer.back()));

      (*this).back() += buffer;
      ++(cursor_.col_iter);
    }
  }
};


#if __cplusplus < 201703L
} // namespace posix
} // namespace meevax
#else
} // namespace meevax::posix
#endif


  //   icurses.header.back() += meevax::ansi_escape_sequence::color::foreground::green;
  //   icurses.header.back() += "meevax@";
  //   icurses.header.back() += boost::asio::ip::host_name();
  //   icurses.header.back() += ": ";
  //   icurses.header.back() += meevax::ansi_escape_sequence::color::foreground::yellow;
  //   icurses.header.back() += std::experimental::filesystem::current_path().string();
  //   icurses.header.back() += meevax::ansi_escape_sequence::color::foreground::white;
  //   icurses.header.back() += "$ ";
  //   icurses.header.back() += meevax::ansi_escape_sequence::attributes::off;


#endif

