#ifndef INCLUDED_MEEVAX_POSIX_IOCTL_HPP
#define INCLUDED_MEEVAX_POSIX_IOCTL_HPP


#include <system_error>
#include <utility>

#include <sys/ioctl.h>


namespace meevax::posix {


template <typename... Ts>
decltype(auto) ioctl(Ts&&... args) noexcept(false)
{
  return io_control(std::forward<Ts>(args)...);
}


template <typename... Ts>
void io_control(Ts&&... args) noexcept(false)
{
  if (::ioctl(std::forward<Ts>(args)...) < 0)
  {
    throw std::system_error {errno, std::system_category()};
  }
}


} // namespace meevax::posix


#endif

