#ifndef INCLUDED_MEEVAX_SEMANTICS_OBJECTIVE_HPP
#define INCLUDED_MEEVAX_SEMANTICS_OBJECTIVE_HPP


/**
* @file objective.hpp
*
* Provides classes that express semantics of objective case.
*/


#include <type_traits>
#include <utility>

#include <meevax/concepts/is_char_type.hpp>
#include <meevax/concepts/is_standard_container.hpp>


/**
* Semantics for a character.
* Modules of header "meevax/semantics/..." are in.
*/
namespace meevax::semantics {


/**
* @class objective objective.hpp <meevax/semantics/objective.hpp>
*
* Base class for semantics classes.
*
* @tparam Scalar this type requires following concepts
* @code
*   std::is_scalar<ScalarType>::value == true
* @endcode
*/
template <
#ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  template <typename...> typename SemanticSemiosis,
  typename ScalarType,
  typename = typename std::enable_if<
                        std::is_scalar<ScalarType>::value
                      >::type
#else
  template <typename...> typename SemanticSemiosis,
  typename ScalarType
#endif // #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
>
class objective
{
  template <typename... Ts>
  using semantic_semiosis = SemanticSemiosis<Ts...>;

  using semantic_scope    = ScalarType;

public:
  /**
  * Type definition for metafunction.
  */
  using value_type = semantic_scope;

public:
  operator const value_type&()
  {
    return buffer;
  }

protected:
  /**
  * Static internal buffer.
  */
  static inline value_type buffer {};
};


} // namespace meevax::semantics


#endif // #ifndef INCLUDED_MEEVAX_SEMANTICS_OBJECTIVE_HPP

