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

#ifndef NDEBUG
    std::cout << "[finder] ";
    for (const auto& c : target)
    {
      if (&c == &*iter && &c == &*last) { std::cout << "\e[0m\e[1;33;42m"; }
      else if (&c == &*iter) { std::cout << "\e[0m\e[1;33m"; }
      else if (&c == &*last) { std::cout << "\e[0m\e[1;42m"; }
      else { std::cout << "\e[0m"; }
      std::cout << (c == '\e' ? "\\e" : c == '\n' ? "\\n" : std::string {c});
    }
    std::cout << "\e[0m\n";
#endif

    result.emplace_back(iter != target.begin() ? iter - 1 : iter, last);
    iter = (last != target.end() ? ++last : last);

#ifndef NDEBUG
    std::cout << "[update] ";
    for (const auto& c : target)
    {
      if (&c == &*iter && &c == &*last) { std::cout << "\e[0m\e[1;33;42m"; }
      else if (&c == &*iter) { std::cout << "\e[0m\e[1;33m"; }
      else if (&c == &*last) { std::cout << "\e[0m\e[1;42m"; }
      else { std::cout << "\e[0m"; }
      std::cout << (c == '\e' ? "\\e" : c == '\n' ? "\\n" : std::string {c});
    }
    std::cout << "\e[0m\n";
#endif
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
