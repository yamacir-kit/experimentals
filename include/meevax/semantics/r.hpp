#ifndef INCLUDED_MEEVAX_SEMANTICS_R_HPP
#define INCLUDED_MEEVAX_SEMANTICS_R_HPP


/**
* @file r.hpp
*
* Provides classes that express semantics for a character \b `r` implementation.
*/


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
#include <meevax/semantics/objective.hpp>


/**
* Semantics for a character.
* Modules of header "meevax/semantics/..." are in.
*/
namespace meevax::semantics {


/**
* @class r_ r.hpp <meevax/semantics/r.hpp>
*
* General template for a character \b `r` implementation.
* This class must not be instantiated.
*/
template <
#ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  typename SemanticScope, typename = void
#else
  typename SemanticScope
#endif // #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
>
class r_;


/**
* @class r_<CharType> r.hpp <meevax/semantics/r.hpp>
*
* Template specialization for `CharType`.
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
#endif // #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
>
  : public meevax::semantics::objective<
             meevax::semantics::r_, CharType
           >
{
  /**
  * Type definition for simplify the description.
  */
  using objective = meevax::semantics::objective<
                      meevax::semantics::r_, CharType
                    >;

  using typename objective::value_type;
  using          objective::buffer;

public:
  /**
  * Read a character into internal buffer from file descriptor.
  *
  * @param fd
  * @returns const reference to static internal buffer.
  */
  const decltype(auto) operator()(int fd = STDIN_FILENO)
  {
    return 0 < ::read(fd, &buffer, sizeof buffer) ?  buffer : throw std::system_error {errno, std::system_category()};
  }

  /**
  * Read a character into internal buffer from standard input stream.
  *
  * @param istream
  * @returns const reference to static internal buffer.
  */
  const decltype(auto) operator()(std::basic_istream<value_type>& istream)
  {
    return objective::buffer = static_cast<value_type>(istream.get());
  }
};


/**
* @class r_<StandardContainer> r.hpp <meevax/semantics/r.hpp>
*
* Template specialization for `StandardContainer`.
*
* @tparam StandardContainer this type requires following concepts
* @code
*   meevax::concepts::is_standard_container<StandardContainer>::value == true
* @endcode
*/
template <typename StandardContainer>
class r_<
#ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  StandardContainer,
  typename std::enable_if<
             meevax::concepts::is_standard_container<StandardContainer>::value
           >::type
#else
  StandardContainer
#endif // #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
>
  : public meevax::semantics::objective<
             meevax::semantics::r_, StandardContainer
           >
{
public: // types
  /**
  * type definition for metafunction.
  */
  using value_type = StandardContainer;

private: // attribs
  /**
  * static internal buffer.
  */
  static inline value_type buffer_;

public:
  /**
  * read a line into internal buffer from standard input stream.
  * this function enable if the template parameter StandardContainer is equivalent
  * to std::basic_string<CharType>.
  *
  * @tparam CharType this type requires following concepts
  * @code
  *   meevax::concepts::is_char_type<CharType>::value == true
  * @endcode
  * @code
  *   std::is_same<std::basic_string<CharType>, StandardContainer>::value == true
  * @endcode
  *
  * @param istream
  * @returns const reference to internal buffer.
  */
  template <
#ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
    typename CharType = typename StandardContainer::value_type,
    typename = typename std::enable_if<
                          meevax::concepts::is_char_type<CharType>::value
                        >::type,
    typename = typename std::enable_if<
                          std::is_same<
                            std::basic_string<CharType>,
                            StandardContainer
                          >::value
                        >::type
#else
    typename CharType = typename StandardContainer::value_type
#endif // #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  >
  const decltype(auto) operator()(std::basic_istream<CharType>& istream = std::cin)
  {
    std::getline(istream, buffer_);
    return buffer_;
  }
};


/**
* Helper template variable for class r_.
* This template variable is useful when you want to use the function object class r_
* like a free function.
*/
template <typename... Ts>
meevax::semantics::r_<Ts...> r;


} // namespace meevax::semantics


#endif // #ifndef INCLUDED_MEEVAX_SEMANTICS_R_HPP

