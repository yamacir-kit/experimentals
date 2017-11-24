#ifndef INCLUDED_MEEVAX_POSIX_READ_HPP
#define INCLUDED_MEEVAX_POSIX_READ_HPP


#include <cassert>
#include <system_error>

#include <unistd.h>


namespace meevax::posix {


template <typename Char>
auto read(int fd) noexcept(false)
{
  assert(::isatty(fd));

  static Char buffer {};

  if (::read(fd, &buffer, sizeof buffer) == -1)
  {
    throw std::system_error {errno, std::system_category()};
  }

  return buffer;
}


} // namespace meevax::posix


#endif

