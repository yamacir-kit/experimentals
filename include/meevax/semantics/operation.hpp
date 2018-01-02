#ifndef INCLUDED_MEEVAX_SEMANTICS_OPERATION_HPP
#define INCLUDED_MEEVAX_SEMANTICS_OPERATION_HPP


#include <type_traits>


namespace meevax::semantics {


template <typename SemanticSemiosis>
class operation
{
public:
  [[deprecated]] decltype(auto) ready()
  {
    return static_cast<SemanticSemiosis&>(this)->ready();
  }

  template <typename... Ts>
  decltype(auto) operator()(Ts&&... args)
  {
  }
};


} // namespace meevax::semantics


#endif // #ifndef INCLUDED_MEEVAX_SEMANTICS_OPERATION_HPP

