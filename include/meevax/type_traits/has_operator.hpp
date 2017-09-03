#ifndef INCLUDED_MEEVAX_TYPE_TRAITS_HAS_OPERATOR_HPP
#define INCLUDED_MEEVAX_TYPE_TRAITS_HAS_OPERATOR_HPP


#include <type_traits>
#include <utility>


namespace meevax {


class has_function_call_operator_
{
public:
  template <typename Functor, typename... Ts>
  static constexpr auto check(Functor&& functor, Ts&&... args) noexcept
    -> decltype(functor(std::forward<Ts>(args)...), std::true_type {})
  {
    return {};
  }

  template <typename Functor>
  static constexpr decltype(auto) check(...) noexcept
  {
    return std::false_type {};
  }
};


template <typename T, typename... Ts>
class has_function_call_operator
  : public decltype(has_function_call_operator_::check<T>(
             std::forward<T>(std::declval<T>()), std::forward<Ts>(std::declval<Ts>())...
            ))
{};


} // namespace meevax


#endif

