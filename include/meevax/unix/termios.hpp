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

public:
  enum class posix_input_flags // p1368
    : decltype(::termios::c_iflag)
  {
    brkint = BRKINT,
    icrnl  = ICRNL,
    ignbrk = IGNBRK,
    igncr  = IGNCR,
    ignpar = IGNPAR,
    inlcr  = INLCR,
    inpck  = INPCK,
    istrip = ISTRIP,
    ixany  = IXANY,
    ixoff  = IXOFF,
    ixon   = IXON,
    parmrk = PARMRK,
  };

  enum class posix_output_flags
    : decltype(::termios::c_oflag)
  {
    bsdly  = BSDLY,
    crdly  = CRDLY,
    ffdly  = FFDLY,
    nldly  = NLDLY,
    ocrnl  = OCRNL,
    ofdel  = OFDEL,
    ofill  = OFILL,
    olcuc  = OLCUC,
    onlcr  = ONLCR,
    onlret = ONLRET,
    onocr  = ONOCR,
    opost  = OPOST,
    tabdly = TABDLY,
    vtdly  = VTDLY,
  };

  enum class posix_control_flags
    : decltype(::termios::c_cflag)
  {
    cbaud   = CBAUD,
    cbaudex = CBAUDEX,
    cibaud  = CIBAUD,
    clocal  = CLOCAL,
    cmspar  = CMSPAR,
    cread   = CREAD,
    crtscts = CRTSCTS,
    csize   = CSIZE,
    cstopb  = CSTOPB,
    hupcl   = HUPCL,
    parenb  = PARENB,
    parodd  = PARODD,
  };

  enum class posix_local_flags
    : decltype(::termios::c_lflag)
  {
    echo    = ECHO,
    echoctl = ECHOCTL,
    echoe   = ECHOE,
    echok   = ECHOK,
    echoke  = ECHOKE,
    echonl  = ECHONL,
    echoprt = ECHOPRT,
    flusho  = FLUSHO,
    icanon  = ICANON,
    iexten  = IEXTEN,
    isig    = ISIG,
    noflsh  = NOFLSH,
    pendin  = PENDIN,
    tostop  = TOSTOP,
    xcase   = XCASE,
  };
};


} // namespace unix


#endif
