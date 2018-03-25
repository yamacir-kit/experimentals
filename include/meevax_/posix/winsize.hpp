#ifndef INCLUDED_MEEVAX_POSIX_WINSIZE_HPP
#define INCLUDED_MEEVAX_POSIX_WINSIZE_HPP


#include <system_error>

#include <signal.h>
#include <unistd.h>

#include <meevax/posix/ioctl.hpp>


namespace meevax::posix {


class winsize // TODO SIGNAL HANDLING
  : public ::winsize
{
  const int fd_;

  static constexpr auto request_ {TIOCGWINSZ};

public:
  explicit winsize(int fd)
    : fd_ {::isatty(fd) ? fd : throw std::system_error {errno, std::system_category()}}
  {
    meevax::posix::io_control(fd_, request_, this);
  }
};


} // namespace meevax::posix


#endif

