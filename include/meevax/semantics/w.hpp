#ifndef INCLUDED_MEEVAX_SEMANTICS_W_HPP
#define INCLUDED_MEEVAX_SEMANTICS_W_HPP


/**
* @file
*
* Provides classes that express semantics for a character @b `'w'` implementation.
* To do more detail description.
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
#include <meevax/semantics/semiosis.hpp>


/**
* Semantics for a character.
* Modules of header "meevax/semantics/..." are in.
*/
namespace meevax::semantics {


/**
* @headerfile w.hpp <meevax/semantics/w.hpp>
*
* Template specialization for semiosis @b `'w'` and semantic scope @b `CharType`.
*
* @tparam CharType this type requires following concepts
* @code
*   meevax::concepts::is_char_type<CharType>::value == true
* @endcode
*/
template <typename CharType>
class semiosis<'w', CharType
#ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
, typename std::enable_if<
             meevax::concepts::is_char_type<CharType>::value
           >::type
#endif // #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
>
  : public meevax::semantics::objective<'w', CharType>
{
  /**
  * Type definition for simplify the description.
  */
  using objective = meevax::semantics::objective<'w', CharType>;

public:
  decltype(auto) operator()()
  {
    return std::cout << "[debug] operator()\n";
  }

  decltype(auto) operator()(CharType&& objective)
  {
    return std::cout << objective << std::flush;
  }
};


MEEVAX_SEMANTICS_SEMIOSIS_HELPER_CLASS_TEMPLATE(w)
MEEVAX_SEMANTICS_SEMIOSIS_HELPER_CLASS_TEMPLATE_DEDUCTION_GUIDE(w)
MEEVAX_SEMANTICS_SEMIOSIS_HELPER_VARIABLE_TEMPLATE(w)


} // namespace meevax::semantics


#endif // #ifndef INCLUDED_MEEVAX_SEMANTICS_W_HPP

