#ifndef INCLUDED_MEEVAX_TYPE_TRAITS_IS_CHAR_TYPE_HPP
#define INCLUDED_MEEVAX_TYPE_TRAITS_IS_CHAR_TYPE_HPP


/**
* @file is_char_type.hpp
*
* 何故か標準ライブラリにないメタ関数 `is_char_type` を提供するヘッダファイル。
* それ以上のことはない。
*/


#include <type_traits>


namespace meevax::type_traits
{
  /**
  * 与えられたテンプレートパラメータが規格が言うところの文字型であるかを判定するメタ関数。
  *
  * このメタ関数が `true` を返すということは `std::basic_string` の特殊化が存在しているということを意味する。<br>
  * もちろん、`char` 以外が現実的に使われないことは知っている上で作った。
  */
  template <typename NonCharType>
  class is_char_type
    : public std::false_type
  {};


  /// テンプレート特殊化。`std::true_type` から派生する。
  template <>
  class is_char_type<char>
    : public std::true_type
  {};


  /// テンプレート特殊化。`std::true_type` から派生する。
  template <>
  class is_char_type<char16_t>
    : public std::true_type
  {};


  /// テンプレート特殊化。`std::true_type` から派生する。
  template <>
  class is_char_type<char32_t>
    : public std::true_type
  {};


  /// テンプレート特殊化。`std::true_type` から派生する。
  template <>
  class is_char_type<wchar_t>
    : public std::true_type
  {};
} // namespace meevax::type_traits


#endif // #ifndef INCLUDED_MEEVAX_TYPE_TRAITS_IS_CHAR_TYPE_HPP

