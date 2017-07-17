#ifndef INCLUDED_MEEVAX_UTILITY_PAIRED_POINTS_HPP_
#define INCLUDED_MEEVAX_UTILITY_PAIRED_POINTS_HPP_


#include <utility>


namespace meevax::utility {


template <typename T, typename U = T>
class paired_points
  : std::pair<T,U>
{
public:
  T& x;
  U& y;

  template <typename... Ts>
  explicit paired_points(Ts&&... args)
    : std::pair<T,T> {std::forward<Ts>(args)...},
      x {(*this).first},
      y {(*this).second}
  {}
};


} // namespace meevax::utility


#endif

