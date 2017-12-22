#ifndef INCLUDED_MEEVAX_SEMANTICS_W_HPP
#define INCLUDED_MEEVAX_SEMANTICS_W_HPP


/**
* @file w.hpp
*
* Provides classes that express semantics for a character \b `w` implementation.
*/


#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <system_error> // std::system_error
#include <type_traits>
#include <utility>

#include <unistd.h> // {annonymous}::write(...)

#include <meevax/concepts/is_char_type.hpp>
#include <meevax/concepts/is_standard_container.hpp>
#include <meevax/semantics/objective.hpp>
#include <meevax/string/runtime_typename.hpp>


/**
* Semantics for a character.
* Modules of header "meevax/semantics/..." are in.
*/
namespace meevax::semantics {


/**
* @class w_ w.hpp <meevax/semantics/w.hpp>
*
* General template for a character \b `w` implementation.
* This class must not be instantiated.
*/
template <
#ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  typename SemanticScope, typename = void
#else
  typename SemanticScope
#endif // #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
>
class w_;


/**
* @class w_<CharType> w.hpp <meevax/semantics/w.hpp>
*
* Template specialization for CharType.
*
* @tparam CharType this type requires following concepts
* @code
*   meevax::concepts::is_char_type<CharType>::value == true
* @endcode
*/
template <typename CharType>
class w_<
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
             meevax::semantics::w_, CharType
           >
{
  /**
  * Type definition for simplify the description.
  */
  using objective = meevax::semantics::objective<meevax::semantics::w_, CharType>;

public:
  template <template <typename...> typename SemanticSemiosis, typename SemanticScope>
  w_(meevax::semantics::objective<SemanticSemiosis, SemanticScope>&&)
  {
    std::cout << "[debug] " << meevax::string::runtime_typename(*this) << "\n";
  }

  decltype(auto) operator()()
  {
    return std::cout << "[debug] operator()\n";
  }
};


template <template <typename...> typename SemanticSemiosis, typename SemanticScope>
w_(meevax::semantics::objective<SemanticSemiosis, SemanticScope>&&)
  -> w_<typename meevax::semantics::objective<
                   SemanticSemiosis, SemanticScope
                 >::value_type>;


template <typename... Ts>
meevax::semantics::w_<Ts...> w;


} // namespace meevax::semantics


#endif // #ifndef INCLUDED_MEEVAX_SEMANTICS_W_HPP

