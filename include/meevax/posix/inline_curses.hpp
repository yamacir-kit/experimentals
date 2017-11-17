#ifndef INCLUDED_MEEVAX_POSIX_INLINE_CURSES_HPP
#define INCLUDED_MEEVAX_POSIX_INLINE_CURSES_HPP


#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <list>
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


template <typename Char = char>
class inline_curses
  : public ::winsize,
    public std::list<std::basic_string<Char>>
{
public:
  using char_type = Char;

private:
  const int fd_;

  std::pair<
    typename std::list<std::basic_string<char_type>>::iterator,
    typename std::basic_string<char_type>::iterator
  > cursor_;

public:
  explicit inline_curses(int fd) noexcept(false)
    : std::list<std::basic_string<char_type>> {"\e[0;38;5;059mready, you have control.\e[0m"},
      fd_ {::isatty(fd) ? fd : throw std::system_error {errno, std::system_category()}},
      cursor_ {std::begin(*this), std::begin(*std::begin(*this))}
  {
    update();
  }

  void draw(std::ostream& ostream)
  {
    ostream << "\e[?7l" << std::flush;

    for (std::size_t row {0}; row < (*this).size(); ++row)
    {
      ostream << "\r\e[K" << (row + 1 != (*this).size() ? "\e[A" : "");
    }

    for (auto iter {std::begin(*this)}; iter != std::end(*this); ++iter)
    {
      ostream << std::setw(std::to_string((*this).size()).size()) << std::right << std::distance(std::begin(*this), iter) << " ";
      ostream << *iter << (std::distance(&(*this).back(), &*iter) != 0 ? "\n" : "");
    }

    ostream << "\e[?7h" << std::flush;
  }

  decltype(auto) read() noexcept(false)
  {
    std::basic_string<char_type> buffer {meevax::posix::read<char_type>(fd_)};

    switch (buffer.back())
    {
    case 0x1B:
      while (!std::regex_match(buffer, std::regex {"^\\\e\\[(\\d*;?)+(.)$"}))
      {
        buffer.push_back(meevax::posix::read<char_type>(fd_));
      }
      break;

    case '\n':
      (*this).emplace_back("");
      ++(cursor_.first);

      break;

    default:
      assert(std::isprint(buffer.back()));

      (*this).back() += buffer;
      ++(cursor_.second);
    }
  }

private:
  void update() noexcept // TODO SIGNAL HANDLER
  {
    ::ioctl(fd_, TIOCGWINSZ, this);
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

