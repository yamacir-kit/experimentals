#ifndef INCLUDED_MEEVAX_TYPE_TRAITS_VOID_TYPE_HPP
#define INCLUDED_MEEVAX_TYPE_TRAITS_VOID_TYPE_HPP


#include <type_traits>


#if __cplusplus < 201703L
namespace meevax {
namespace type_traits {
#else
namespace meevax::type_traits {
#endif


#if __cplusplus < 201703L
template <typename...>
using void_type = void;
#else
template <typename... Ts>
using void_type = std::void_t<Ts...>;
#endif



#if __cplusplus < 201703L
} // namespace type_traits
} // namespace meevax
#else
} // namespace meevax::type_traits
#endif


#endif

