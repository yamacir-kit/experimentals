#ifndef INCLUDED_MEEVAX_ALGORITHM_NEARER_HPP
#define INCLUDED_MEEVAX_ALGORITHM_NEARER_HPP


#include <algorithm>
#include <iterator>


#if __cplusplus < 201703L
namespace meevax {
namespace algorithm {
#else
namespace meevax::algorithm {
#endif


template <typename InputIterator>
auto nearer(InputIterator begin, InputIterator a, InputIterator b)
{
  if (std::distance(begin, a) < std::distance(begin, b))
  {
    return a;
  }
  else
  {
    return b;
  }
}


#if __cplusplus < 201703L
} // namespace algorithm
} // namespace meevax
#else
} // namespace meevax::algorithm
#endif


#endif

