#ifndef INCLUDED_MEEVAX_ALGORITHM_REGEX_SPLIT_INCLUDE_DELIMITER_HPP_
#define INCLUDED_MEEVAX_ALGORITHM_REGEX_SPLIT_INCLUDE_DELIMITER_HPP_


#include <algorithm>


namespace meevax::algorithm {


template <template <typename...> typename SequenceContainer, typename C>
void regex_split_include_delimiter(
       SequenceContainer<std::basic_string<C>>& split_results,
       const std::basic_string<C>& target,
       const SequenceContainer<std::basic_regex<C>>& regex_delimiters)
{
  std::string wait;

  std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
  std::match_results<
    typename std::basic_string<C>::const_iterator
  > match_results, temporary_match_results, empty_results;

  std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
  auto iter {std::begin(target)};

  std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
  while (iter != std::end(target))
  {
    std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
    match_results = empty_results;

    std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
    for (const auto& regex : regex_delimiters)
    {
      std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
      if (std::regex_search(iter, std::end(target), temporary_match_results, regex))
      {
        if (!match_results.ready())
        {
          std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
          match_results = temporary_match_results;
        }
        else if (temporary_match_results.position() < match_results.position())
        {
          std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
          match_results = temporary_match_results;
        }
        else
        {
          std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
        }

        if (match_results.ready())
        {
          std::cout << "[debug] match_res.pos(): " << match_results.position()
                          << ", tempo_res.pos(): " << temporary_match_results.position() << std::endl;
        }
      }
    }

    std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);

    if (match_results.ready())
    {
      std::cout << "[debug] find: " << match_results.str()
                            << ", " << match_results.position() << "\n";

      std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
      split_results.emplace_back(iter,
                                 iter + match_results.position());

      std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
      split_results.emplace_back(iter + match_results.position(),
                                 iter + match_results.position() + match_results.length());

      std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
      iter = iter + match_results.position() + match_results.length();
      std::cout << "[debug] iter: " << std::distance(std::begin(target), iter) << "\n\n";
    }
    else
    {
      split_results.emplace_back(std::begin(target), std::end(target));
      return;
    }
  }
}


} // namespace meevax::algorithm


#endif
