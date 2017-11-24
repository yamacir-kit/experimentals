#ifndef INCLUDED_MEEVAX_POSIX_TERMIOS_HPP
#define INCLUDED_MEEVAX_POSIX_TERMIOS_HPP


#include <cerrno>
#include <system_error>

#include <termios.h>
#include <unistd.h>


namespace meevax::posix {


class termios
  : public ::termios
{
  const int fd_;
  struct ::termios default_;

public:
  explicit termios(int fd)
    : fd_ {::isatty(fd) ? fd : throw std::system_error {errno, std::system_category()}},
      default_ {(::tcgetattr(fd_, this), *this)}
  {}

  ~termios()
  {
    ::tcsetattr(fd_, TCSANOW, &default_);
  }

  decltype(auto) set(int optional_actions = TCSANOW) const noexcept
  {
    return ::tcsetattr(fd_, optional_actions, this);
  }
};


} // namespace meevax::posix


#endif

