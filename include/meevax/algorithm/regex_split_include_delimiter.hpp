#ifndef INCLUDED_MEEVAX_ALGORITHM_REGEX_SPLIT_INCLUDE_DELIMITER_HPP_
#define INCLUDED_MEEVAX_ALGORITHM_REGEX_SPLIT_INCLUDE_DELIMITER_HPP_


#include <algorithm>
#include <regex>
#include <string>


namespace meevax::algorithm {


template <template <typename...> typename SequenceContainer, typename C>
inline void regex_split_include_delimiter(
              SequenceContainer<std::basic_string<C>>& split_results,
              const std::basic_string<C>& target,
              const SequenceContainer<std::basic_regex<C>>& regex_delimiters)
{
  static std::match_results<
    typename std::basic_string<C>::const_iterator
  > match_results, temporary_match_results, empty_results;

  for (auto iter {std::begin(target)}; iter != std::end(target); iter += match_results.position() + match_results.length())
  {
    match_results = empty_results;

    for (const auto& regex : regex_delimiters)
    {
      if (std::regex_search(iter, std::end(target), temporary_match_results, regex))
      {
        if (!match_results.ready() || temporary_match_results.position() < match_results.position())
        {
          match_results = temporary_match_results;
        }
      }
    }

    if (match_results.ready())
    {
      split_results.emplace_back(iter,
                                 iter + match_results.position());

      split_results.emplace_back(iter + match_results.position(),
                                 iter + match_results.position() + match_results.length());
    }
    else
    {
      split_results.emplace_back(iter, std::end(target));
      return;
    }
  }
}


} // namespace meevax::algorithm


#endif
