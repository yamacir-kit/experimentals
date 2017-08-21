#ifndef INCLUDED_MEEVAX_STRING_STATIC_CONCAT_HPP
#define INCLUDED_MEEVAX_STRING_STATIC_CONCAT_HPP


#include <array>
#include <type_traits>
#include <utility>

#include <meevax/type_traits/has_value_type.hpp>


namespace meevax {


template <typename T, std::size_t N>
constexpr decltype(auto) static_array_size_(const T(&)[N]) noexcept
{
  return typename std::conditional<
                    std::is_same<T, char>::value, // TODO ADD OTHER CHAR TYPE (wchar_t, uchar16, uchar32)
                    std::integral_constant<std::size_t, N - 1>,
                    std::integral_constant<std::size_t, N>
                  >::type {};
}


template <typename T, std::size_t N>
constexpr decltype(auto) static_array_size_(const std::array<T,N>&) noexcept
{
  return typename std::conditional<
                    std::is_same<T, char>::value, // TODO ADD OTHER CHAR TYPE (wchar_t, uchar16, uchar32)
                    std::integral_constant<std::size_t, N - 1>,
                    std::integral_constant<std::size_t, N>
                  >::type {};
}


template <typename T>
using static_array_size = decltype(meevax::static_array_size_(std::declval<T>()));


template <typename T, typename U, std::size_t... Ts, std::size_t... Us>
constexpr auto static_concat_(const T& lhs, const U& rhs,
                              std::integer_sequence<std::size_t, Ts...>,
                              std::integer_sequence<std::size_t, Us...>) noexcept
  -> std::array<
       typename std::remove_reference<decltype(std::declval<T>()[0])>::type,
       meevax::static_array_size<T>::value + meevax::static_array_size<U>::value
     >
{
  return {{lhs[Ts]..., rhs[Us]...}};
}


template <typename T, typename U = const typename std::remove_reference<decltype(std::declval<T>()[0])>::type(&)[1]>
constexpr decltype(auto) static_concat(T&& lhs, U&& rhs = "") noexcept
{
  return meevax::static_concat_(
    std::forward<T>(lhs), std::forward<U>(rhs),
    std::make_integer_sequence<std::size_t, meevax::static_array_size<T>::value> {},
    std::make_integer_sequence<std::size_t, meevax::static_array_size<U>::value> {}
  );
}


template <typename T, typename U, typename... Ts>
constexpr decltype(auto) static_concat(T&& lhs, U&& rhs, Ts&&... args) noexcept
{
  return meevax::static_concat(
    std::forward<T>(lhs),
    meevax::static_concat(std::forward<U>(rhs), std::forward<Ts>(args)...)
  );
}


} // namespace meevax::string


#endif

