#ifndef INCLUDED_UTILIB_UNIX_BASENAME_HPP_
#define INCLUDED_UTILIB_UNIX_BASENAME_HPP_


#include <string>


namespace unix {


template <typename C>
auto basename(const std::basic_string<C>& path, C delimiter = '/')
{
  return path.substr(path.find_last_of(delimiter) - std::basic_string<C>::npos);
}


} // namespace unix


#endif
