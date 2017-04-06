#ifndef INCLUDED_MEEVAX_UNIX_SHELL_HPP_
#define INCLUDED_MEEVAX_UNIX_SHELL_HPP_


#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <system_error>
#include <vector>

#include "meevax/cmake_config.hpp"

#include "meevax/unix/basename.hpp"
#include "meevax/unix/execvp.hpp"
#include "meevax/unix/fork.hpp"


template <typename C>
class static_concatenate
{
public:
  using char_type = typename std::basic_string<C>::value_type;
  using size_type = typename std::basic_string<char_type>::size_type;

  template <typename T, typename U>
  constexpr auto operator()(T&& lhs, U&& rhs = "") noexcept
  { return cat_(std::forward<T>(lhs), std::forward<U>(rhs), mkixseq<size<T>::value-1>(), mkixseq<size<U>::value>()); }

  template <typename T, typename U, typename... Ts>
  constexpr auto operator()(T&& lhs, U&& rhs, Ts&&... args) noexcept
  { return operator()(std::forward<T>(lhs), operator()(std::forward<U>(rhs), std::forward<Ts>(args)...)); }

private:
  template <size_type... Ts>
  using ixseq = std::integer_sequence<size_type, Ts...>;

  template <size_type N>
  using mkixseq = std::make_integer_sequence<size_type, N>;

protected:
  template <typename T, size_type N>
  static constexpr auto size_(const T(&)[N]) noexcept
    -> std::integral_constant<size_type,N>
  { return {}; }

  template <typename T, size_type N>
  static constexpr auto size_(const std::array<T,N>&) noexcept
    -> std::integral_constant<size_type,N>
  { return {}; }

  template <typename T>
  using size = decltype(size_(std::declval<T>()));

  template <typename T, typename U, size_type... Ix1, size_type... Ix2>
  static constexpr auto cat_(const T& lhs, const U& rhs, ixseq<Ix1...>, ixseq<Ix2...>) noexcept
    -> std::array<char_type, size<T>::value + size<U>::value - 1>
  { return {{lhs[Ix1]..., rhs[Ix2]...}}; }
};


namespace unix {


template <typename C>
class shell
{
public:
  using char_type = typename std::basic_string<C>::value_type;
  using size_type = typename std::basic_string<char_type>::size_type;

private:
  const std::vector<std::basic_string<char_type>> argv_;
        std::vector<std::basic_string<char_type>> input_;

  static constexpr auto version_ {static_concatenate<char_type>()("version ", PROJECT_VERSION, " alpha")};

public:
  explicit shell(int argc, char** argv)
    : argv_ {argv, argv + argc},
      input_ {}
  {
    for (auto iter {argv_.begin()}; iter != argv_.end(); ++iter)
    {
      for (const auto& s : std::vector<std::basic_string<char_type>> {"-h", "--help"})
      {
        if (std::regex_match(*iter, std::basic_regex<char_type>{s}))
        {
          // for (const auto& v : help(argv_))
          // {
          //   for (const auto& s : v) std::cout << s << (&s != &v.back() ? ' ' : '\n');
          // }

          std::cout << "[debug] help\n";
        }
      }

      for (const auto& s : std::vector<std::basic_string<char_type>> {"-v", "--version"})
      {
        if (std::regex_match(*iter, std::basic_regex<char_type>{s}))
        {
          // for (const auto& s : version()) std::cout << s << ' ';
          // std::cout << std::endl;

          std::cout << "[debug] version\n";
        }
      }
    }
  };

  int run() // XXX UGLY CODE !!!
  {
    std::cout << unix::basename(argv_[0]) << "$ ";

    for (std::string buffer; !std::getline(std::cin, buffer).eof(); input_.clear())
    {
      for (std::stringstream input {buffer}; std::getline(input, buffer, ' '); input_.push_back(buffer));

      if (input_[0] == "exit") { return 0; }

      else if (input_[0] == "help")
      {
        std::cout << "[debug] help\n";
      }

      try { unix::fork()(unix::execvp<char_type>(input_)); }

      catch (std::system_error&) { throw; }

      catch (...) { throw; }

      std::cout << unix::basename(argv_[0]) << "$ ";
    }

    return 0;
  }

  const auto input() const noexcept { return input_; }

protected:
  // static auto version()
  //   -> std::vector<std::basic_string<char_type>>
  // {
  //   return {{"version"}, {PROJECT_VERSION}, {"alpha"}};
  // }

  // auto help(const std::vector<std::basic_string<char_type>>& argv) // UGLY CODE !!!
  //   -> std::vector<std::vector<std::basic_string<char_type>>>
  // {
  //   return {
  //     {unix::basename(argv[0]), {"shell"}, {"-"}, version()},
  //     {{}},
  //     {{"USAGE:"}, unix::basename(argv[0]), {"[options]"}},
  //     {{}},
  //     {{"\t"}, {"-h"}, {"--help"},    {"\t"}, {"display this help"}},
  //     {{"\t"}, {"-v"}, {"--version"}, {"\t"}, {"display version information"}}
  //   };
  // }
};


} // namespace unix


#endif
