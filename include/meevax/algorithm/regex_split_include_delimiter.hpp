#ifndef INCLUDED_MEEVAX_ALGORITHM_REGEX_SPLIT_INCLUDE_DELIMITER_HPP_
#define INCLUDED_MEEVAX_ALGORITHM_REGEX_SPLIT_INCLUDE_DELIMITER_HPP_


#include <algorithm>


namespace meevax::algorithm {


// template <template <typename...> typename SequenceContainer, typename C>
// void regex_split_include_delimiter(
//        SequenceContainer<std::basic_string<C>>& split_results,
//        const std::basic_string<C>& target,
//        const SequenceContainer<std::basic_regex<C>>& regex_delimiters)
// {
//   // std::string wait;
//
//   // std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
//   std::match_results<
//     typename std::basic_string<C>::const_iterator
//   > match_results, temporary_match_results, empty_results;
//
//   // std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
//   auto iter {std::begin(target)};
//
//   // std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
//   while (iter != std::end(target))
//   {
//     // std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
//     match_results = empty_results;
//
//     // std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
//     for (const auto& regex : regex_delimiters)
//     {
//       // std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
//       if (std::regex_search(iter, std::end(target), temporary_match_results, regex))
//       {
//         if (!match_results.ready())
//         {
//           // std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
//           match_results = temporary_match_results;
//         }
//         else if (temporary_match_results.position() < match_results.position())
//         {
//           // std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
//           match_results = temporary_match_results;
//         }
//         // else
//         // {
//         //   std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
//         // }
//
//         // if (match_results.ready())
//         // {
//         //   std::cout << "[debug] match_res.pos(): " << match_results.position()
//         //                   << ", tempo_res.pos(): " << temporary_match_results.position() << std::endl;
//         // }
//       }
//     }
//
//     // std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
//
//     if (match_results.ready())
//     {
//       // std::cout << "[debug] find: " << match_results.str()
//       //                       << ", " << match_results.position() << "\n";
//
//       // std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
//       split_results.emplace_back(iter,
//                                  iter + match_results.position());
//
//       // std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
//       split_results.emplace_back(iter + match_results.position(),
//                                  iter + match_results.position() + match_results.length());
//
//       // std::cout << "[debug] line: " << __LINE__ << std::endl; std::getline(std::cin, wait);
//       iter = iter + match_results.position() + match_results.length();
//       // std::cout << "[debug] iter: " << std::distance(std::begin(target), iter) << "\n\n";
//     }
//     else
//     {
//       split_results.emplace_back(std::begin(target), std::end(target));
//       return;
//     }
//   }
// }


template <template <typename...> typename SequenceContainer, typename C>
void regex_split_include_delimiter(
       SequenceContainer<std::basic_string<C>>& split_results,
       const std::basic_string<C>& target,
       const SequenceContainer<std::basic_regex<C>>& regex_delimiters)
{
  std::match_results<
    typename std::basic_string<C>::const_iterator
  > match_results, temporary_match_results, empty_results;

  auto iter {std::begin(target)};

  while (iter != std::end(target))
  {
    match_results = empty_results;

    for (const auto& regex : regex_delimiters)
    {
      if (std::regex_search(iter, std::end(target), temporary_match_results, regex))
      {
        if (!match_results.ready())
        {
          std::cout << "[debug] line: " << __LINE__ << std::endl;
          match_results = temporary_match_results;
        }
        else if (temporary_match_results.position() < match_results.position())
        {
          std::cout << "[debug] line: " << __LINE__ << std::endl;
          match_results = temporary_match_results;
        }
        else
        {
          std::cout << "\e[0;31m[debug] line: \e[0m" << __LINE__ << std::endl;
        }

        if (match_results.ready())
        {
          std::cout << "[debug] match_res.pos(): " << match_results.position()
                          << ", tempo_res.pos(): " << temporary_match_results.position() << std::endl;
        }
      }
    }

    if (match_results.ready())
    {
      std::string s {match_results.str()};
      std::cout << "[debug] find: ";
      if (s[0] == '\e')
      {
        std::cout << "\\e" << std::string {std::begin(s) + 1, std::end(s)};
      }
      else if (s[0] == '\n')
      {
        std::cout << "\\n" << std::string {std::begin(s) + 1, std::end(s)};
      }
      else
      {
        std::cout << s;
      }
      std::cout << ", " << match_results.position() << "\n";

      split_results.emplace_back(iter,
                                 iter + match_results.position());

      split_results.emplace_back(iter + match_results.position(),
                                 iter + match_results.position() + match_results.length());

      iter = iter + match_results.position() + match_results.length();
      std::cout << "[debug] iter: " << std::distance(std::begin(target), iter) << "\n\n";
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
