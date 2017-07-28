#ifndef INCLUDED_MEEVAX_UTILITY_RENAMED_PAIR_HPP_
#define INCLUDED_MEEVAX_UTILITY_RENAMED_PAIR_HPP_


#include <utility>


namespace meevax::utility::renamed_pair {


template <typename T, typename U = T>
class point
  : std::pair<T,U>
{
public:
  T& x;
  U& y;

  template <typename... Ts>
  explicit point(Ts&&... args)
    : std::pair<T,T> {std::forward<Ts>(args)...},
      x {(*this).first},
      y {(*this).second}
  {}
};


} // namespace meevax::utility::renamed_pair


#endif

