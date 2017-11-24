#ifndef INCLUDED_MEEVAX_POSIX_INLINE_CURSES_HPP
#define INCLUDED_MEEVAX_POSIX_INLINE_CURSES_HPP


#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <regex>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

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
  const int fd_;

  std::size_t window_upper_left_row_;
  std::size_t window_upper_left_column_;

  std::size_t relative_cursor_row_;
  std::size_t relative_cursor_column_;

  std::size_t previous_row_;

public:
  using char_type = CharType;

public:
  inline_curses(int fd, const std::basic_string<char_type>&& s = "") noexcept(false)
    : SequenceContainer<std::basic_string<char_type>> {std::forward<decltype(s)>(s)},
      fd_ {::isatty(fd) ? fd : throw std::system_error {errno, std::system_category()}}
  {
    ::ioctl(fd_, TIOCGWINSZ, this);

    window_upper_left_row_ = 0;
    window_upper_left_column_ = 0;

    relative_cursor_row_ = 0;
    relative_cursor_column_ = 0;

    previous_row_ = 0;
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
      break;

    default:
      assert(std::isprint(buffer.back()));
      (*this).back() += buffer;
    }
  }

  void write(std::basic_ostream<char_type>& ostream)
  {
    ostream << "\e[?7h" << std::flush;

    auto iter {std::begin(*this)}; std::advance(iter, window_upper_left_column_);

    for (auto row {std::min(previous_row_, static_cast<std::size_t>((*this).ws_row))}; 0 < row; --row)
    {
      ostream << "\e[A";
    }
    previous_row_ = 0;

    for (const auto first {iter};
         iter != std::end(*this) && std::distance(first, iter) < (*this).ws_row - 1;
         ++iter)
    {
      ostream << "\r\e[K";

      ostream << std::setw(std::to_string((*this).size()).size())
              << std::right
              << std::distance(std::begin(*this), iter)
              << " ";

      ostream << *iter << "\n";

      ++previous_row_;
    }

    ostream << std::right
            << "position: [" << window_upper_left_row_ + relative_cursor_row_ << ", "
            << window_upper_left_column_ + relative_cursor_column_ << "]";

    ostream << "\e[?7l" << std::flush;
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

