#ifndef INCLUDED_MEEVAX_TRIAL_STATIC_CONCATENATE_HPP_
#define INCLUDED_MEEVAX_TRIAL_STATIC_CONCATENATE_HPP_


#include <array>
#include <string>
#include <utility>
#include <type_traits>


namespace trial {


template <typename C>
class static_concatenate
{
public:
  using char_type = typename std::basic_string<C>::value_type;
  using size_type = typename std::basic_string<char_type>::size_type;

  template <typename T, typename U = const C(&)[1]>
  constexpr auto operator()(T&& lhs, U&& rhs = "") const noexcept
  { return cat_(std::forward<T>(lhs), std::forward<U>(rhs), mkixseq<size<T>::value-1>(), mkixseq<size<U>::value>()); }

  template <typename T, typename U, typename... Ts>
  constexpr auto operator()(T&& lhs, U&& rhs, Ts&&... args) const noexcept
  { return operator()(std::forward<T>(lhs), operator()(std::forward<U>(rhs), std::forward<Ts>(args)...)); }

private:
  template <size_type... Ts>
  using ixseq = std::integer_sequence<size_type, Ts...>;

  template <size_type N>
  using mkixseq = std::make_integer_sequence<size_type, N>;

protected:
  template <typename T, size_type N>
  static constexpr auto size_(const T(&)[N]) noexcept
    -> std::integral_constant<size_type,N>
  { return {}; }

  template <typename T, size_type N>
  static constexpr auto size_(const std::array<T,N>&) noexcept
    -> std::integral_constant<size_type,N>
  { return {}; }

  template <typename T>
  using size = decltype(size_(std::declval<T>()));

  template <typename T, typename U, size_type... Ix1, size_type... Ix2>
  static constexpr auto cat_(const T& lhs, const U& rhs, ixseq<Ix1...>, ixseq<Ix2...>) noexcept
    -> std::array<char_type, size<T>::value + size<U>::value - 1>
  { return {{lhs[Ix1]..., rhs[Ix2]...}}; }
};


} // namespace trial


#endif
