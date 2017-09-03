#ifndef INCLUDED_MEEVAX_UTILITY_RENAMED_PAIR_HPP_
#define INCLUDED_MEEVAX_UTILITY_RENAMED_PAIR_HPP_


#include <utility>


#define MEEVAX_UTILITY_RENAMED_PAIR(TOKEN, FIRST, SECOND) \
template <typename T, typename U = T> \
class TOKEN \
  : std::pair<T,U> \
{ \
public: \
  T& FIRST; \
  U& SECOND; \
\
  template <typename... Ts> \
  explicit TOKEN(Ts&&... args) \
    : std::pair<T,U> {std::forward<Ts>(args)...}, \
      FIRST  {(*this).first}, \
      SECOND {(*this).second} \
  {} \
};



namespace meevax::utility::renamed_pair {


MEEVAX_UTILITY_RENAMED_PAIR(point, x, y);


} // namespace meevax::utility::renamed_pair


#undef MEEVAX_UTILITY_RENAMED_PAIR


#endif

