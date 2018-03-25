#ifndef INCLUDED_MEEVAX_TYPE_TRAITS_IS_CHAR_TYPE_HPP
#define INCLUDED_MEEVAX_TYPE_TRAITS_IS_CHAR_TYPE_HPP


#include <string>
#include <type_traits>


namespace meevax::type_traits
{
  template <typename NonCharType>
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
} // namespace meevax::type_traits


#endif // #ifndef INCLUDED_MEEVAX_TYPE_TRAITS_IS_CHAR_TYPE_HPP

