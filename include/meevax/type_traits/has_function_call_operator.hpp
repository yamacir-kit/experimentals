#ifndef MEEVAX_TYPE_TRAITS_HAS_FUNCTION_CALL_OPERATOR_HPP
#define MEEVAX_TYPE_TRAITS_HAS_FUNCTION_CALL_OPERATOR_HPP


#include <type_traits>


namespace meevax {


namespace has_function_call_operator_
{
  template <typename Functor>
  constexpr auto check(Functor&& functor)
    -> decltype(functor(), std::true_type {})
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


template <typename T>
using has_function_call_operator
  = decltype(has_function_call_operator_::check<T>(std::declval<T>{}));


} // namespace meevax


#endif

