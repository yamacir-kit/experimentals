#ifndef INCLUDED_MEEVAX_SEMANTICS_OBJECTIVE_HPP
#define INCLUDED_MEEVAX_SEMANTICS_OBJECTIVE_HPP


/**
* @file objective.hpp
*
* Provides classes that express semantics of objective case.
*/


#include <type_traits>
#include <utility>

// #include <meevax/concepts/is_char_type.hpp>
// #include <meevax/concepts/is_standard_container.hpp>


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
* @tparam SemanticSemiosis this type requires no concepts.
* @tparam SemanticScope    this type requires no concepts.
*/
template <auto SemanticSemiosis, typename SemanticScope>
class objective
{
public:
  static constexpr auto semantic_semiosis {SemanticSemiosis};

  using semantic_scope = SemanticScope;

  /**
  * Type definition for metafunction.
  */
  using value_type = semantic_scope;

  using char_type [[deprecated]] = decltype(SemanticSemiosis);

protected:
  /**
  * Static internal buffer.
  */
  static inline value_type buffer {};

public:
  operator const value_type&()
  {
    return buffer;
  }
};


} // namespace meevax::semantics


#endif // #ifndef INCLUDED_MEEVAX_SEMANTICS_OBJECTIVE_HPP

