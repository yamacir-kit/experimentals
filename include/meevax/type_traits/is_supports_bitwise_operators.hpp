#ifndef INCLUDED_MEEVAX_TYPE_TRAITS_IS_SUPPORTS_BITWISE_OPERATORS_HPP
#define INCLUDED_MEEVAX_TYPE_TRAITS_IS_SUPPORTS_BITWISE_OPERATORS_HPP


/**
* @file is_supports_bitwise_operators.hpp
*
* スコープを持つ列挙型（enum class）へのビット演算を提供するするためのヘルパクラス群。
* 型安全なビットマスクが欲しかった。
*
* 本体はメタ関数 `is_scoped_bitmask`。
* それ以外はこのメタ関数が `true` を返すことをコンセプトとする
* 無名名前空間内で定義されたビット演算子オーバーロードである。
*/


#include <type_traits>


namespace meevax::type_traits
{
  /**
  * テンプレートパラメータの型がビット幅演算をサポートしているか否かを返すメタ関数。
  *
  * デフォルトでは常に `std::false_type` から派生する。
  * つまり、ビット演算をサポートするスコープを持つ列挙型を作成した場合、
  * そのクラスに対するこのメタ関数の特殊化を定義しておかなければならない。
  */
  template <typename T>
  class is_supports_bitwise_operators
    : public std::false_type
  {};
} // namespace meevax::type_traits


namespace // {anonnymous}
{
  /**
  * ビット和演算子のオーバーロード。
  *
  * @tparam EnumClass
  *   このテンプレートパラメータは下記のコンセプトを満たすことを要求する。
  * @code
  *   meevax::type_traits::is_supports_bitwise_operators<EnumClass>::value == true
  * @endcode
  * @code
  *   std::is_enum<EnumClass>::value == true
  * @endcode
  *
  * @param lhs 左辺値。
  * @param rhs 右辺値。
  *
  * @return ビット和の演算結果。型は `EnumClass`。
  */
  template <typename EnumClass
  #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  , typename = typename std::enable_if<
                          meevax::type_traits::is_supports_bitwise_operators<EnumClass>::value
                        >::type
  , typename = typename std::enable_if<
                          std::is_enum<EnumClass>::value
                        >::type
  #endif
  >
  constexpr auto operator|(const EnumClass& lhs, const EnumClass& rhs) noexcept
  {
    return static_cast<EnumClass>(
      static_cast<typename std::underlying_type<EnumClass>::type>(lhs) |
      static_cast<typename std::underlying_type<EnumClass>::type>(rhs)
    );
  }


  /**
  * ビット和代入演算子のオーバーロード。
  *
  * @tparam EnumClass
  *   このテンプレートパラメータは下記のコンセプトを満たすことを要求する。
  * @code
  *   meevax::type_traits::is_supports_bitwise_operators<EnumClass>::value == true
  * @endcode
  * @code
  *   std::is_enum<EnumClass>::value == true
  * @endcode
  *
  * @param lhs 左辺値。
  * @param rhs 右辺値。
  *
  * @return 左辺値への参照。
  */
  template <typename EnumClass
  #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  , typename = typename std::enable_if<
                          meevax::type_traits::is_supports_bitwise_operators<EnumClass>::value
                        >::type
  , typename = typename std::enable_if<
                          std::is_enum<EnumClass>::value
                        >::type
  #endif
  >
  constexpr auto& operator|=(const EnumClass& lhs, const EnumClass& rhs) noexcept
  {
    return lhs = lhs | rhs;
  }


  /**
  * ビット積演算子のオーバーロード。
  *
  * @tparam EnumClass
  *   このテンプレートパラメータは下記のコンセプトを満たすことを要求する。
  * @code
  *   meevax::type_traits::is_supports_bitwise_operators<EnumClass>::value == true
  * @endcode
  * @code
  *   std::is_enum<EnumClass>::value == true
  * @endcode
  *
  * @param lhs 左辺値。
  * @param rhs 右辺値。
  *
  * @return ビット積の演算結果。型は `EnumClass`。
  */
  template <typename EnumClass
  #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  , typename = typename std::enable_if<
                          meevax::type_traits::is_supports_bitwise_operators<EnumClass>::value
                        >::type
  , typename = typename std::enable_if<
                          std::is_enum<EnumClass>::value
                        >::type
  #endif
  >
  constexpr auto operator&(const EnumClass& lhs, const EnumClass& rhs) noexcept
  {
    return static_cast<EnumClass>(
      static_cast<typename std::underlying_type<EnumClass>::type>(lhs) &
      static_cast<typename std::underlying_type<EnumClass>::type>(rhs)
    );
  }


  /**
  * ビット積代入演算子のオーバーロード。
  *
  * @tparam EnumClass
  *   このテンプレートパラメータは下記のコンセプトを満たすことを要求する。
  * @code
  *   meevax::type_traits::is_supports_bitwise_operators<EnumClass>::value == true
  * @endcode
  * @code
  *   std::is_enum<EnumClass>::value == true
  * @endcode
  *
  * @param lhs 左辺値。
  * @param rhs 右辺値。
  *
  * @return 左辺値への参照。
  */
  template <typename EnumClass
  #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  , typename = typename std::enable_if<
                          meevax::type_traits::is_supports_bitwise_operators<EnumClass>::value
                        >::type
  , typename = typename std::enable_if<
                          std::is_enum<EnumClass>::value
                        >::type
  #endif
  >
  constexpr auto& operator&=(const EnumClass& lhs, const EnumClass& rhs) noexcept
  {
    return lhs = lhs & rhs;
  }


  /**
  * ビット否定演算子のオーバーロード。
  *
  * @tparam EnumClass
  *   このテンプレートパラメータは下記のコンセプトを満たすことを要求する。
  * @code
  *   meevax::type_traits::is_supports_bitwise_operators<EnumClass>::value == true
  * @endcode
  * @code
  *   std::is_enum<EnumClass>::value == true
  * @endcode
  *
  * @param arg 演算対象の列挙型の値。
  *
  * @return ビット否定の演算結果。型は `EnumClass`。
  */
  template <typename EnumClass
  #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  , typename = typename std::enable_if<
                          meevax::type_traits::is_supports_bitwise_operators<EnumClass>::value
                        >::type
  , typename = typename std::enable_if<
                          std::is_enum<EnumClass>::value
                        >::type
  #endif
  >
  constexpr auto operator~(const EnumClass& arg) noexcept
  {
    return static_cast<EnumClass>(
      ~static_cast<typename std::underlying_type<EnumClass>::type>(arg)
    );
  }
} // namespace {annonymous}


#endif // INCLUDED_MEEVAX_TYPE_TRAITS_IS_SUPPORTS_BITWISE_OPERATORS_HPP

