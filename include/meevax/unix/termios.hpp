#ifndef INCLUDED_MEEVAX_UNIX_TERMIOS_HPP_
#define INCLUDED_MEEVAX_UNIX_TERMIOS_HPP_


extern "C" {
#include <termios.h>
#include <unistd.h>
}


namespace unix {


class termios
  : private ::termios
{
  int fd_;

public:
  explicit termios(int fd)
    : fd_ {::isatty(fd) != 0 ? fd : -1}
  {
    ::tcgetattr(fd_, this); // terminal control get attribute
  }
};


} // namespace unix


#endif
