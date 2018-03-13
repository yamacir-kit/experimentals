#ifndef INCLUDED_MEEVAX_SEMANTICS_SEMIOSIS_HPP
#define INCLUDED_MEEVAX_SEMANTICS_SEMIOSIS_HPP


/**
* @file semiosis.hpp
* @brief Provides base class that express semantics for any character.
*/


#include <iostream>
#include <type_traits>
#include <utility>

#include <meevax/string/runtime_typename.hpp>


/**
* Semantics for a character.
* Modules of header "meevax/semantics/..." are in.
*/
namespace meevax::semantics {


class semiosis_base
{
public:
  virtual void operator()() = 0;
  virtual std::string description() = 0;
};


/**
* @headerfile semiosis.hpp <meevax/semantics/semiosis.hpp>
* @brief General template class for semantics represents any character and semantic scope.
*/
template <auto SemanticSemiosis, typename SemanticScope
#ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
, typename = void
#endif
>
class semiosis
  : public semiosis_base
{
public:
  void operator()()
  {
    std::cout << "[debug] unimplemented semiosis: " << meevax::string::runtime_typename(*this) << "\n" << std::flush;
  }
};


/**
* @brief Object format macro to assist class definition.
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
  {} \
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
meevax::semantics::SEMIOSIS<Ts...> SEMIOSIS##_; \


} // namespace meevax::semantics


#endif

