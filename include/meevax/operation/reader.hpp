/**
* @file operation/reader.hpp
*
* The header file which provides generic data read operation.
* This instantiate reader class object for `/dev/stdin` named `read` implicitly.
*/

#ifndef INCLUDED_MEEVAX_OPERATION_READ_HPP
#define INCLUDED_MEEVAX_OPERATION_READ_HPP


#include <iostream>
#include <string>
#include <system_error>

#include <unistd.h>

#include <meevax/posix/file_descriptor.hpp>
#include <meevax/posix/termios.hpp>
#include <meevax/type_traits/is_char_type.hpp>


namespace meevax::operation
{
  template <typename T
  #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  , typename = typename std::enable_if<
                          meevax::type_traits::is_char_type<T>::value
                        >::type
  #endif // DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  >
  class reader
    : public std::basic_string<T>
  {
  public:
    using char_type = typename std::basic_string<T>::value_type;

  private:
    meevax::posix::file_descriptor fd_;
    meevax::posix::termios termios_;

  public:
    reader(meevax::posix::file_descriptor&& fd, bool noncanonical = false)
      : fd_ {std::move(fd)},
        termios_ {fd_}
    {
      using namespace meevax::posix;

      if (noncanonical and fd_.is_tty())
      {
        termios_.change_to(termios::input_mode::noncanonical);
      }
    }

  public:
    decltype(auto) operator()() noexcept(false)
    {
      static decltype(std::getchar()) buffer {};

      if (::read(fd_, &buffer, sizeof buffer) < 0)
      {
        throw std::system_error {errno, std::system_category()};
      }
      else return static_cast<char_type>(buffer);
    }
  };

  static reader<char> read {meevax::posix::fd::stdin, true};
} // namespace meevax::operation


#endif // INCLUDED_MEEVAX_OPERATION_READ_HPP

