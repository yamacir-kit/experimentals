#ifndef INCLUDED_MEEVAX_SEMANTICS_R_HPP
#define INCLUDED_MEEVAX_SEMANTICS_R_HPP


static_assert(
  201703L <= __cplusplus,
  "\e[1;32mmodule \"meevax/semantics/r.hpp\" will be available in C++17 or later.\e[0m"
);


#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <system_error> // std::system_error
#include <type_traits>
#include <utility>

#include <unistd.h> // {annonymous}::read(...)

#include <meevax/concepts/is_char_type.hpp>
#include <meevax/concepts/is_standard_container.hpp>


#if __cplusplus < 201703L
namespace meevax {
namespace semantics {
#else
namespace meevax::semantics {
#endif


// TODO
// それぞれにコンストラクタをきちんと実装、
// クラステンプレートのテンプレートパラメータ推論から構築できるようにすること


template <typename SemanticScope, typename = void>
class r_;


template <typename CharType>
class r_<CharType,
         typename std::enable_if<
                    meevax::concepts::is_char_type<CharType>::value
                  >::type>
{
  using char_type = CharType;

public:
  decltype(auto) operator()(int fd = STDIN_FILENO)
  {
    char_type buffer {};
    return 0 < ::read(fd, &buffer, sizeof buffer) ? buffer : throw std::system_error {errno, std::system_category()};
  }

  decltype(auto) operator()(std::basic_istream<char_type>& istream)
  {
    return static_cast<char_type>(istream.get());
  }
};


template <typename CharType>
class r_<std::basic_string<CharType>,
         typename std::enable_if<
                    meevax::concepts::is_char_type<CharType>::value
                  >::type>
{
  using char_type = CharType;

public:
  decltype(auto) operator()(std::basic_istream<char_type>& istream = std::cin, char_type delimiter = '\n')
  {
    std::basic_string<char_type> buffer {};
    std::getline(istream, buffer, delimiter);

    return std::move(buffer);
  }
};


// template <typename StandardContainer>
// class r_<StandardContainer,
//          typename std::enable_if<
//                     meevax::concepts::is_standard_container<StandardContainer>::value
//                   >::type>
// {
// public:
//   template <typename CharType,
//             typename = typename std::enable_if<
//                                   meevax::concepts::is_char_type<CharType>::value
//                                 >::type>
//   decltype(auto) operator()(std::basic_istream<CharType>& istream = std::cin)
//   {
//     StandardContainer buffer {};
//
//     for ()
//     {
//       buffer.emplace(
//         std::end(buffer),
//         meevax::semantics::r<typename StandardContainer::value_type>(
//           std::forward<decltype(istream)>(istream)
//         )
//       );
//     }
//   }
// };


template <typename... Ts>
meevax::semantics::r_<Ts...> r;


#if __cplusplus < 201703L
} // namespace semantics
} // namespace meevax
#else
} // namespace meevax::semantics
#endif


#endif // #ifndef INCLUDED_MEEVAX_SEMANTICS_R_HPP

