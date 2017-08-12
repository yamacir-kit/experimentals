#ifndef MEEVAX_TYPE_TRAITS_HAS_FUNCTION_CALL_OPERATOR_HPP
#define MEEVAX_TYPE_TRAITS_HAS_FUNCTION_CALL_OPERATOR_HPP


#include <type_traits>
#include <utility>


namespace meevax {


namespace has_function_call_operator_
{
  template <typename Functor, typename... Ts>
  constexpr auto check(Functor&& functor, Ts&&... args)
    -> decltype(functor(std::forward<Ts>(args)...), std::true_type {})
  {
    return {};
  }

  template <typename Functor>
  constexpr auto check(...)
    -> std::false_type
  {
    return {};
  }
} // namespace has_function_call_operator_


template <typename T, typename... Ts>
using has_function_call_operator
  = decltype(
      has_function_call_operator_::check<T>(
        std::forward<T>(std::declval<T>()),
        std::forward<Ts>(std::declval<Ts>())...
      )
    );


} // namespace meevax


#endif

