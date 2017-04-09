#ifndef INCLUDED_MEEVAX_UNIX_TERMIOS_HPP_
#define INCLUDED_MEEVAX_UNIX_TERMIOS_HPP_


extern "C" {
#include <unistd.h>
#include <termios.h>
}


namespace unix {


class termios
  : private ::termios
{};


} // namespace unix


#endif
