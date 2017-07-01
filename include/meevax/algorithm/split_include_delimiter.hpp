#ifndef INCLUDED_MEEVAX_ALGORITHM_SPLIT_INCLUDE_DELIMITER_HPP_
#define INCLUDED_MEEVAX_ALGORITHM_SPLIT_INCLUDE_DELIMITER_HPP_


#include <algorithm>


namespace meevax {
  namespace algorithm {


// TODO THIS MUST BE MORE EFFICIENTLY

template <template <typename...> class V, typename S>
void split_include_delimiter(V<S>& result, const S& target, S&& delims)
{
  for (auto iter {target.begin()}; iter != target.end(); )
  {
    auto last {std::find_first_of(iter, target.end(), delims.begin(), delims.end())};

    result.emplace_back(iter != target.begin() ? iter - 1 : iter, last);
    iter = (last != target.end() ? ++last : last);
  }

  for (const auto& c : delims)
  {
    if (target.back() == c)
    {
      result.emplace_back(target.end() - 1, target.end());
    }
  }
}


  } // namespace algorithm
} // namespace meevax


#endif
