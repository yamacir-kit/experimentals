#ifndef INCLUDED_MEEVAX_TYPE_TRAITS_IS_SCOPED_BITMASKS_HPP
#define INCLUDED_MEEVAX_TYPE_TRAITS_IS_SCOPED_BITMASKS_HPP


/**
* @file is_scoped_bitmasks.hpp
*
* ビット演算可能なスコープを持つ列挙型（enum class）を作成するためのヘルパクラス群。
*
* 本体はメタ関数 `is_scoped_bitmask` であり、
* それ以外はこのメタ関数が `true` を返すことをコンセプトとする
* 無名名前空間内で定義されたビット演算子オーバーロードである。
*/


#include <type_traits>


/**
* 標準ライブラリ `type_traits` の拡張メタ関数群を含む名前空間。
*/
namespace meevax::type_traits
{
  /**
  * テンプレートパラメータの型がビット幅演算をサポートしているか否かを返すメタ関数。
  *
  * デフォルトでは常に `std::false_type` から派生する。<br>
  * つまり、ビット演算をサポートするスコープを持つ列挙型を作成した場合、
  * そのクラスに対するこのメタ関数の特殊化を定義しておかなければならない。
  *
  * なお、このメタ関数が `true` を返す列挙型に関して、
  * 各メンバの値が `2^n` の要件を満たしているか否かはチェックされない。
  */
  template <typename T>
  class is_scoped_bitmasks
    : public std::false_type
  {};
} // namespace meevax::type_traits


namespace // {anonnymous}
{
  /**
  * ビット和演算子のオーバーロード。
  *
  * @tparam ScopedBitmasks
  *   このテンプレートパラメータは下記のコンセプトを満たすことを要求する。
  * @code
  *   meevax::type_traits::is_scoped_bitmasks<ScopedBitmasks>::value == true
  * @endcode
  */
  template <typename ScopedBitmasks
  #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  , typename = typename std::enable_if<
                          meevax::type_traits::is_scoped_bitmasks<ScopedBitmasks>::value
                        >::type
  #endif
  >
  constexpr auto operator|(const ScopedBitmasks& lhs, const ScopedBitmasks& rhs) noexcept
  {
    return static_cast<ScopedBitmasks>(
      static_cast<typename std::underlying_type<ScopedBitmasks>::type>(lhs) |
      static_cast<typename std::underlying_type<ScopedBitmasks>::type>(rhs)
    );
  }


  /**
  * ビット和代入演算子のオーバーロード。
  *
  * @tparam ScopedBitmasks
  *   このテンプレートパラメータは下記のコンセプトを満たすことを要求する。
  * @code
  *   meevax::type_traits::is_scoped_bitmasks<ScopedBitmasks>::value == true
  * @endcode
  */
  template <typename ScopedBitmasks
  #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  , typename = typename std::enable_if<
                          meevax::type_traits::is_scoped_bitmasks<ScopedBitmasks>::value
                        >::type
  #endif
  >
  constexpr auto& operator|=(const ScopedBitmasks& lhs, const ScopedBitmasks& rhs) noexcept
  {
    return lhs = lhs | rhs;
  }


  /**
  * ビット積演算子のオーバーロード。
  *
  * @tparam ScopedBitmasks
  *   このテンプレートパラメータは下記のコンセプトを満たすことを要求する。
  * @code
  *   meevax::type_traits::is_scoped_bitmasks<ScopedBitmasks>::value == true
  * @endcode
  */
  template <typename ScopedBitmasks
  #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  , typename = typename std::enable_if<
                          meevax::type_traits::is_scoped_bitmasks<ScopedBitmasks>::value
                        >::type
  #endif
  >
  constexpr auto operator&(const ScopedBitmasks& lhs, const ScopedBitmasks& rhs) noexcept
  {
    return static_cast<ScopedBitmasks>(
      static_cast<typename std::underlying_type<ScopedBitmasks>::type>(lhs) &
      static_cast<typename std::underlying_type<ScopedBitmasks>::type>(rhs)
    );
  }


  /**
  * ビット積代入演算子のオーバーロード。
  *
  * @tparam ScopedBitmasks
  *   このテンプレートパラメータは下記のコンセプトを満たすことを要求する。
  * @code
  *   meevax::type_traits::is_scoped_bitmasks<ScopedBitmasks>::value == true
  * @endcode
  */
  template <typename ScopedBitmasks
  #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  , typename = typename std::enable_if<
                          meevax::type_traits::is_scoped_bitmasks<ScopedBitmasks>::value
                        >::type
  #endif
  >
  constexpr auto& operator&=(const ScopedBitmasks& lhs, const ScopedBitmasks& rhs) noexcept
  {
    return lhs = lhs & rhs;
  }


  /**
  * ビット否定演算子のオーバーロード。
  *
  * @tparam ScopedBitmasks
  *   このテンプレートパラメータは下記のコンセプトを満たすことを要求する。
  * @code
  *   meevax::type_traits::is_scoped_bitmasks<ScopedBitmasks>::value == true
  * @endcode
  */
  template <typename ScopedBitmasks
  #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  , typename = typename std::enable_if<
                          meevax::type_traits::is_scoped_bitmasks<ScopedBitmasks>::value
                        >::type
  #endif
  >
  constexpr auto operator~(const ScopedBitmasks& arg) noexcept
  {
    return static_cast<ScopedBitmasks>(
      ~static_cast<typename std::underlying_type<ScopedBitmasks>::type>(arg)
    );
  }
} // namespace {annonymous}


#endif // INCLUDED_MEEVAX_TYPE_TRAITS_IS_SCOPED_BITMASKS_HPP

