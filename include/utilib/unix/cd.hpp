#ifndef INCLUDED_UTILIB_UNIX_CD_HPP_
#define INCLUDED_UTILIB_UNIX_CD_HPP_


#include <iostream>
#include <string>
#include <system_error>

#include <unistd.h>


namespace unix {


template <typename C>
auto cd(const std::basic_string<C>& path)
{
  if (::chdir(path.c_str()) != 0)
  {
    try
    {
      throw std::system_error {errno, std::system_category()};
    }

    catch (const std::system_error& error)
    {
      std::cerr << "[error] code: " << error.code().value() << " - " << error.code().message() << std::endl;
    }

    return false;
  }

  return true;
}


} // namespace unix


#endif
