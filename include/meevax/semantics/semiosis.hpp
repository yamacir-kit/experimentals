#ifndef INCLUDED_MEEVAX_SEMANTICS_SEMIOSIS_HPP
#define INCLUDED_MEEVAX_SEMANTICS_SEMIOSIS_HPP


/**
* @file semiosis.hpp
*
* Provides base class that express semantics for any character.
* To do more detail description.
*/


#include <type_traits>

#include <meevax/string/runtime_typename.hpp>


/**
* Semantics for a character.
* Modules of header "meevax/semantics/..." are in.
*/
namespace meevax::semantics {


/**
* @headerfile semiosis.hpp <meevax/semantics/semiosis.hpp>
*
* general template class for semantics represents any character and semantic scope.
*/
template <auto SemanticSemiosis, typename SemanticScope, typename = void>
class semiosis;


/**
* Object format macro to assist class definition.
*/
#define MEEVAX_SEMANTICS_SEMIOSIS_HELPER_CLASS_TEMPLATE(SEMIOSIS) \
template <typename SemanticScope> \
class SEMIOSIS \
  : public meevax::semantics::semiosis<#SEMIOSIS[0], SemanticScope> \
{ \
public: \
  explicit SEMIOSIS() = default; \
\
  template <typename SemanticObjective> \
  explicit SEMIOSIS(SemanticObjective&&) \
  { \
    std::cout << "[debug] instantiated from helper: " \
              << meevax::string::runtime_typename(*this) << "\n"; \
  } \
};


/**
* Object format macro to assist class definition.
*/
#define MEEVAX_SEMANTICS_SEMIOSIS_HELPER_CLASS_TEMPLATE_DEDUCTION_GUIDE(SEMIOSIS) \
template <typename SemanticObjective> \
SEMIOSIS(SemanticObjective&&) \
  -> SEMIOSIS<typename std::remove_reference<SemanticObjective>::type::value_type>;


/**
* Object format macro to assist class definition.
*/
#define MEEVAX_SEMANTICS_SEMIOSIS_HELPER_VARIABLE_TEMPLATE(SEMIOSIS) \
template <typename... Ts> \
[[deprecated]] meevax::semantics::SEMIOSIS<Ts...> SEMIOSIS##_; \


} // namespace meevax::semantics


#endif

