#ifndef INCLUDED_MEEVAX_TRIAL_ARGUMENTS_PARSER_HPP_
#define INCLUDED_MEEVAX_TRIAL_ARGUMENTS_PARSER_HPP_


#define DEBUG_MEEVAX_ARGUMENTS_PARSER_HPP_


#include <string>
#include <vector>


#ifdef DEBUG_MEEVAX_ARGUMENTS_PARSER_HPP_
#include <iostream>
#include <meevax/trial/runtime_typeinfo.hpp>
#endif


namespace trial {


class arguments_parser
{
  std::vector<std::string> argv_;

public:
  explicit arguments_parser(const int argc, const char** argv)
    : argv_ {argv + 1, argv + argc}
  {}

  explicit arguments_parser(std::vector<std::string>&& argv)
    : argv_ {std::forward<std::vector<std::string>>(argv)}
  {
#ifdef DEBUG_MEEVAX_ARGUMENTS_PARSER_HPP_
    std::cout << std::endl
              << "[debug] meevax::arguments_parser::arguments_parser("
              << meevax::runtime_typename(argv) << ")" << std::endl
              << "        ";
    for (const auto& v : argv_) std::cout << v << (&v != &argv_.back() ? ' ' : '\n');
#endif

    auto phase1 {split()};

#ifdef DEBUG_MEEVAX_ARGUMENTS_PARSER_HPP_
    std::cout << std::endl
              << "[debug] meevax::arguments_parser::arguments_parser()" << std::endl
              << "        ";
    for (const auto& p : phase1) std::cout << p << (&p != &phase1.back() ? ' ' : '\n');
#endif
  }

private:
  std::vector<std::string> split()
  {
    std::vector<std::string> result;

    for (auto&& v : argv_) {
      if (v[0] == '-' && v[1] != '-') {
        v.erase(0, 1);

        while (std::isalpha(v[0])) {
          result.emplace_back(&v[0], &v[1]);
          v.erase(0, 1);
        }
      }

      if (v[0] == '-' && v[1] == '-') {
        v.erase(0, 2);

        if (v.find_first_of('=') != std::string::npos) {
          result.emplace_back(&v[0], &v[v.find('=')]);
          v.erase(0, v.find('=') + 1);
        }

        result.emplace_back(v.begin(), v.end());
        v.clear();
      }

      if (!v.empty()) result.emplace_back(v.begin(), v.end());
    }

    return result;
  }
};


} // namespace meevax


#endif
