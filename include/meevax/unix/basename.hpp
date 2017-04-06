#ifndef INCLUDED_MEEVAX_UNIX_BASENAME_HPP_
#define INCLUDED_MEEVAX_UNIX_BASENAME_HPP_


#include <string>


namespace unix {


template <typename C>
auto basename(const std::basic_string<C>& path)
{
  // static_assert(decltype(path)::npos != -1,)
  //
  // static constexpr auto delimiter {'/'};
  //
  //
  // return path.substr()
}


} // namespace unix


#endif
