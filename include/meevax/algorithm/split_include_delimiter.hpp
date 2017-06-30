#ifndef INCLUDED_MEEVAX_ALGORITHM_SPLIT_INCLUDE_DELIMITER_HPP_
#define INCLUDED_MEEVAX_ALGORITHM_SPLIT_INCLUDE_DELIMITER_HPP_


#include <algorithm>


namespace meevax {
  namespace algorithm {


template <template <typename...> class V, typename S>
void split_include_delimiter(V<S>& result, const S& target, S&& delims)
{
  for (auto iter {target.begin()}; iter != target.end(); )
  {
    auto last {std::find_first_of(iter, target.end(), delims.begin(), delims.end())};

    // if (last == target.end())
    // {
    //   std::cout << "[debug] not found\n";
    // }
    // else
    // {
    //   std::cout << "[debug] found: " << std::distance(target.begin(), last) << std::endl;
    // }

    result.emplace_back(iter != target.begin() ? iter - 1 : iter, last);
    iter = (last != target.end() ? ++last : last);
  }
}


  } // namespace algorithm
} // namespace meevax


#endif
