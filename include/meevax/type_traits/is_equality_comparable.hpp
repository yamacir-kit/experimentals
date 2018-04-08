#ifndef INCLUDED_MEEVAX_TYPE_TRAITS_IS_EQUALITY_COMPARABLE_HPP
#define INCLUDED_MEEVAX_TYPE_TRAITS_IS_EQUALITY_COMPARABLE_HPP


/**
* @file is_equality_comparable.hpp
*
* 与えられた型について透過性の比較が可能かどうかを調べるメタ関数を提供するヘッダ。
*/


#include <type_traits>


namespace meevax::type_traits
{
  /**
  * 与えられた型について等価性の比較が可能かどうかを調べるメタ関数。
  */
  template <typename NotEqualityComparable
  #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  , typename = void
  #endif // DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  >
  class is_equality_comparable
    : public std::false_type
  {};

  /**
  * 与えられた型が等価性の比較が可能かどうかを調べるメタ関数。
  */
  template <typename EqualityComparable>
  class is_equality_comparable<EqualityComparable
  #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  , std::void_t<
      decltype(std::declval<EqualityComparable&>() == std::declval<EqualityComparable&>()
    )>
  #endif // DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  >
    : public std::true_type
  {};
}; // meevax::type_traits


#endif // INCLUDED_MEEVAX_TYPE_TRAITS_IS_EQUALITY_COMPARABLE_HPP

