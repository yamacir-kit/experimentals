#ifndef INCLUDED_MEEVAX_TRIAL_GENERIC_PARSER_HPP
#define INCLUDED_MEEVAX_TRIAL_GENERIC_PARSER_HPP


#include <regex>
#include <string>
#include <vector>

template <typename C>
class generic_parser
  : public std::vector<std::basic_string<C>>
{
public:
  explicit generic_parser(const std::vector<std::basic_string<C>>& argv)
    : std::vector<std::basic_string<C>> {argv}
  {
    for (const auto& v : *this) std::cout << v << (&v != &(*this).back() ? ' ' : '\n');
  }

  auto search(const std::vector<std::basic_regex<C>>& regexes)
    -> typename std::vector<std::basic_string<C>>::iterator
  {
    for (auto iter {(*this).begin()}; iter != (*this).end(); ++iter)
    {
      for (const auto& r : regexes)
      {
        if (std::regex_match(*iter, r)) return iter;
      }
    }

    return (*this).end();
  }

  auto search(const std::vector<std::basic_string<C>>& strings)
  {
    return search({strings.begin(), strings.end()});
  }
};


#endif
