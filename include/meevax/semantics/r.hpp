#ifndef INCLUDED_MEEVAX_SEMANTICS_R_HPP
#define INCLUDED_MEEVAX_SEMANTICS_R_HPP


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


/**
* semantics for a character.
* modules of header "meevax/semantics/..." are in.
*/
namespace meevax::semantics {


/**
* General template for character \b r implementation.
* This class must not be instantiated.
*/
template <
#ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  typename SemanticScope, typename = void
#else
  typename SemanticScope
#endif
>
class r_;


/**
* template specialization for CharType.
*
* @tparam CharType this type requires following concepts
* @code
*   meevax::concepts::is_char_type<CharType>::value == true
* @endcode
*/
template <typename CharType>
class r_<
#ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  CharType,
  typename std::enable_if<
             meevax::concepts::is_char_type<CharType>::value
           >::type
#else
  CharType
#endif
>
{
public:
  /// type definition for metafunction.
  using char_type = CharType;

  /// type definition for metafunction.
  using value_type = typename std::basic_string<CharType>::value_type;

private:
  /// internal buffer.
  static inline value_type buffer_;

public:
  /**
   * read a character from file descriptor.
   *
   * @param[in] fd
   * @returns a character read in.
   */
  decltype(auto) operator()(int fd = STDIN_FILENO)
  {
    return 0 < ::read(fd, &buffer_, sizeof buffer_) ? buffer_ : throw std::system_error {errno, std::system_category()};
  }

  /**
   * read a character from standard input stream.
   *
   * @param[in] istream
   * @returns a character read in.
   */
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


} // namespace meevax::semantics


#endif // #ifndef INCLUDED_MEEVAX_SEMANTICS_R_HPP

