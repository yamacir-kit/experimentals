#ifndef INCLUDED_MEEVAX_SEMANTIC_READ_HPP
#define INCLUDED_MEEVAX_SEMANTIC_READ_HPP


static_assert(
  201703L <= __cplusplus,
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


template <typename CharType, typename SemanticRange, typename = void>
class read_;


template <typename CharType>
class read_<CharType, CharType,
            typename std::enable_if<
                       meevax::concepts::is_char_type<CharType>::value
                     >::type>
{
  using char_type = CharType;

public:
  decltype(auto) operator()()
  {
    return static_cast<char_type>(std::getchar());
  }

  decltype(auto) operator()(int fd)
  {
    char_type buffer {};
    return (::read(fd, &buffer, sizeof(char_type)) < 0) ? buffer : throw std::system_error {errno, std::system_category()};
  }

  decltype(auto) operator()(std::basic_istream<char_type>& istream)
  {
    return static_cast<char_type>(istream.get());
  }
};


template <typename CharType>
class read_<CharType, std::basic_string<CharType>,
            typename std::enable_if<
                       meevax::concepts::is_char_type<CharType>::value
                     >::type>
{
  using char_type = CharType;

public:
  decltype(auto) operator()(std::basic_istream<char_type>& istream = std::cin,
                            char_type delimiter = '\n')
  {
    std::basic_string<char_type> buffer {};
    std::getline(istream, buffer, delimiter);

    return std::move(buffer);
  }
};


template <typename... Ts>
read_<Ts...> read {};


#if __cplusplus < 201703L
} // namespace semantic
} // namespace meevax
#else
} // namespace meevax::semantic
#endif


#endif // #ifndef INCLUDED_MEEVAX_SEMANTIC_READ_HPP

