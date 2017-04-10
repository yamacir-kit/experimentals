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

extern "C" {
#include <termios.h>
#include <unistd.h>
}


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

  struct termios default_;

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
          help();
          std::exit(0); // XXX DANGER CODE
        }
      }

      for (const auto& s : std::vector<std::basic_string<char_type>> {"-v", "--version"})
      {
        if (std::regex_match(*iter, std::basic_regex<char_type> {s}))
        {
          std::cout << version() << std::endl;
          std::exit(0); // XXX DANGER CODE
        }
      }
    }

    ::tcgetattr(STDIN_FILENO, &default_);

    struct termios ios {default_};
    ios.c_lflag &= ~(ICANON | ECHO);

    ios.c_cc[VMIN]  = 1;
    ios.c_cc[VTIME] = 0;

    ::tcsetattr(STDIN_FILENO, TCSANOW, &ios);
  };

  ~shell()
  {
    ::tcsetattr(STDIN_FILENO, TCSANOW, &default_);
  }

  [[deprecated]] int run() // XXX UGLY CODE !!!
  {
    std::cout << unix::basename(argv_[0]) << "$ ";

    // for (std::string buffer; !std::getline(std::cin, buffer).eof(); input_.clear())
    for (std::string buffer; false; input_.clear())
    {
      for (std::basic_stringstream<char_type> input {buffer};
           std::getline(input, buffer, ' ');
           input_.push_back(buffer));

      if (input_[0] == "exit") { return 0; }

      else if (input_[0] == "help") { help(); }

      else try { unix::fork()(unix::execvp<char_type>(input_)); }

      catch (std::system_error&) { throw; }

      catch (...) { throw; }

      std::cout << unix::basename(argv_[0]) << "$ ";
    }

    return 0;
  }

  auto led()
  {
    std::basic_string<char_type> word_buffer {};
    std::vector<decltype(word_buffer)> line_buffer {};

    while (true)
    {
      typename decltype(word_buffer)::value_type char_buffer {};
      ::read(STDIN_FILENO, &char_buffer, sizeof(decltype(char_buffer)));

      switch (char_buffer)
      {
        case ' ':
          line_buffer.push_back(word_buffer);
          word_buffer.clear();
          break;

        case '\n':
          if (word_buffer.size() > 0)
          {
            line_buffer.push_back(word_buffer);
            word_buffer.clear();
          }
          return line_buffer;
          break;

        case 127:
          if (word_buffer.size() > 0) { word_buffer.pop_back(); }

          else if (line_buffer.size() > 0)
          {
            word_buffer = line_buffer.back();
            line_buffer.pop_back();
          }

          break;

        default:
          if (std::isgraph(char_buffer))
          {
            word_buffer.push_back(char_buffer);
          }
          break;
      }

      std::cout << "[debug] (" << line_buffer.size() << ": " << word_buffer.size() + 1 << ") ";
      for (const auto& word : line_buffer) { std::cout << word << " "; };
      std::cout << word_buffer << std::endl;
    }
  }

  // const auto input() const noexcept { return input_; }

private:
  static auto version()
  {
    static constexpr auto s {static_concatenate<char_type>()("version ", PROJECT_VERSION, " alpha")};
    return s.data();
  }

  void help() const
  {
    std::cout << unix::basename(argv_[0]) << " shell - " << version() << "\n\n"
              << "USAGE: " << unix::basename(argv_[0]) << " [options]\n\n"
              << "\t-h, --help\tdisplay this help\n"
              << "\t-v, --version\tdisplay version information\n\n";
  }
};


} // namespace unix


#endif
