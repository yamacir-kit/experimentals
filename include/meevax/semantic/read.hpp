#ifndef INCLUDED_MEEVAX_SEMANTIC_READ_HPP
#define INCLUDED_MEEVAX_SEMANTIC_READ_HPP


static_assert(
  201103L <= __cplusplus,
  "module \"meevax/semantic/read.hpp\" will be available in C++11 or later."
);


#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <system_error> // std::system_error
#include <type_traits>

#include <unistd.h> // ::read(...)

#include <meevax/concepts/is_char_type.hpp>
#include <meevax/concepts/is_standard_container.hpp>


#if __cplusplus < 201703L
namespace meevax {
namespace semantic {
#else
namespace meevax::semantic {
#endif


// template <typename...>
// template <typename CharType, typename SemanticRange>
template <typename CharType, typename SemanticRange, typename...>
class read
{
  static_assert(false);
};


template <typename CharType, typename SemanticRange>
class read<CharType, SemanticRange,
           typename std::enable_if<
                      meevax::concepts::is_char_type<CharType>::value
                    >::type,
           typename std::enable_if<
                      std::is_same<
                        CharType, SemanticRange
                      >::value
                    >::type>
{
  using char_type = CharType;

public:
  template <typename... Ts>
  auto operator()(Ts&&... args) -> char_type;

  decltype(auto) operator()() noexcept
  {
    return static_cast<char_type>(std::getchar());
  }

  decltype(auto) operator()(int fd) noexcept(false)
  {
    char_type buffer {};
    return (::read(fd, &buffer, sizeof(char_type)) < 0) ? buffer : throw std::system_error {errno, std::system_category()};
  }

  decltype(auto) operator()(std::basic_istream<char_type>& istream)
  {
    return static_cast<char_type>(istream.get());
  }
};


template <typename CharType, typename SemanticRange>
class read<CharType, SemanticRange,
           typename std::enable_if<
                      meevax::concepts::is_char_type<CharType>::value
                    >::type,
           typename std::enable_if<
                      meevax::concepts::is_standard_container<SemanticRange>::value
                    >::type>
{
  using char_type = CharType;

public:
  template <typename... Ts>
  auto operator()(Ts&&... args) -> SemanticRange;

  template <typename = typename std::enable_if<
                                  std::is_same<
                                    std::basic_string<CharType>, SemanticRange
                                  >::value
                                >::type,
            typename...>
  decltype(auto) operator()(std::basic_istream<char_type>& istream = std::cin,
                            char_type delimiter = '\n')
  {
    SemanticRange buffer {};
    std::getline(istream, buffer, delimiter);

    return std::move(buffer);
  }

  // template <typename = typename std::enable_if<
  //                                 not std::is_same<
  //                                   std::basic_string<CharType>, SemanticRange
  //                                 >::value
  //                               >::type,
  //           typename = typename std::enable_if<
  //                                 std::is_constructible<
  //                                   SemanticRange, std::basic_string<CharType>
  //                                 >::value
  //                               >::type>
  // decltype(auto) operator()(std::basic_istream<char_type>& istream = std::cin,
  //                           char_type delimiter = '\n')
  // {
  //   std::basic_string<CharType> buffer {};
  //   std::getline(istream, buffer, delimiter);
  //
  //   return std::move(SemanticRange {buffer});
  // }

  template <typename = typename std::enable_if<
                                  not std::is_same<
                                    std::basic_string<CharType>, SemanticRange
                                  >::value
                                >::type>
            // typename = typename std::enable_if<
            //                       not std::is_constructible<
            //                         SemanticRange, std::basic_string<CharType>
            //                       >::value
            //                     >::type>
  decltype(auto) operator()(std::basic_istream<char_type>& istream = std::cin,
                            char_type delimiter = '\n')
  {
    std::basic_string<CharType> buffer {};
    std::getline(istream, buffer, delimiter);

    SemanticRange result {};
    std::copy(std::begin(buffer), std::end(buffer), std::back_inserter(result));

    return std::move(result);
  }
};


#if __cplusplus < 201703L
} // namespace semantic
} // namespace meevax
#else
} // namespace meevax::semantic
#endif


#endif // #ifndef INCLUDED_MEEVAX_SEMANTIC_READ_HPP

