#ifndef INCLUDED_MEEVAX_STRING_STATIC_CONCAT_HPP
#define INCLUDED_MEEVAX_STRING_STATIC_CONCAT_HPP


#include <array>
#include <iostream>
#include <type_traits>
#include <utility>

#include <meevax/type_traits/has_type.hpp>


#if __cplusplus < 201703L
namespace meevax {
namespace string {
#else
namespace meevax::string {
#endif


// TODO make metafunction "is_char_type"


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
using static_array_size = decltype(meevax::string::static_array_size_(std::declval<T>()));


template <typename T, typename U, std::size_t... Ts, std::size_t... Us,
          typename = typename std::enable_if<!std::is_same<typename std::remove_reference<decltype(std::declval<T>()[0])>::type, char>::value>::type>
constexpr auto static_concat_(const T& lhs, const U& rhs,
                              std::integer_sequence<std::size_t, Ts...>,
                              std::integer_sequence<std::size_t, Us...>) noexcept
  -> std::array<
       typename std::remove_reference<decltype(std::declval<T>()[0])>::type,
       meevax::string::static_array_size<T>::value + meevax::string::static_array_size<U>::value
     >
{
  return {{lhs[Ts]..., rhs[Us]...}};
}


template <typename T, typename U, std::size_t... Ts, std::size_t... Us>
constexpr auto static_concat_(const T& lhs, const U& rhs,
                              std::integer_sequence<std::size_t, Ts...>,
                              std::integer_sequence<std::size_t, Us...>) noexcept
  -> std::array<
       typename std::remove_reference<decltype(std::declval<T>()[0])>::type,
       meevax::string::static_array_size<T>::value + meevax::string::static_array_size<U>::value + 1
     >
{
  return {{lhs[Ts]..., rhs[Us]..., '\0'}};
}


template <typename T, typename U = const typename std::remove_reference<decltype(std::declval<T>()[0])>::type(&)[1]>
constexpr decltype(auto) static_concat(T&& lhs, U&& rhs = "") noexcept
{
  return meevax::string::static_concat_(
    std::forward<T>(lhs), std::forward<U>(rhs),
    std::make_integer_sequence<std::size_t, meevax::string::static_array_size<T>::value> {},
    std::make_integer_sequence<std::size_t, meevax::string::static_array_size<U>::value> {}
  );
}


template <typename T, typename U, typename... Ts>
constexpr decltype(auto) static_concat(T&& lhs, U&& rhs, Ts&&... args) noexcept
{
  return meevax::string::static_concat(
    std::forward<T>(lhs),
    meevax::string::static_concat(std::forward<U>(rhs), std::forward<Ts>(args)...)
  );
}


#if __cplusplus < 201703L
} // namespace string
} // namespace meevax
#else
} // namespace meevax::string
#endif


template <typename Char, std::size_t N>
inline decltype(auto) operator<<(std::basic_ostream<Char>& lhs, const std::array<Char,N>& rhs)
{
  return lhs << rhs.data();
}


#endif

