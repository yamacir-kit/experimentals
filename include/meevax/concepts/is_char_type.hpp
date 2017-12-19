#ifndef INCLUDED_MEEVAX_CONCEPTS_IS_CHAR_TYPE_HPP
#define INCLUDED_MEEVAX_CONCEPTS_IS_CHAR_TYPE_HPP


static_assert(201103L <= __cplusplus, "module \"meevax/concepts/is_char_type.hpp\" will be available in C++11 or later.");


#include <string>
#include <type_traits>


#if __cplusplus < 201703L
namespace meevax {
namespace concepts {
#else
namespace meevax::concepts {
#endif


template <typename CharType>
class is_char_type
  : public std::false_type
{};


template <>
class is_char_type<char>
  : public std::true_type
{};


template <>
class is_char_type<char16_t>
  : public std::true_type
{};


template <>
class is_char_type<char32_t>
  : public std::true_type
{};


template <>
class is_char_type<wchar_t>
  : public std::true_type
{};


#if __cplusplus < 201703L
} // namespace concepts
} // namespace meevax
#else
} // namespace meevax::concepts
#endif


#endif // #ifndef INCLUDED_MEEVAX_CONCEPTS_IS_CHAR_TYPE_HPP

