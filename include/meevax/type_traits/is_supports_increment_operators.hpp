#ifndef INCLUDED_MEEVAX_TYPE_TRAITS_IS_SUPPORTS_INCREMENT_OPERATORS_HPP
#define INCLUDED_MEEVAX_TYPE_TRAITS_IS_SUPPORTS_INCREMENT_OPERATORS_HPP


/**
* @file is_supports_increment_operators.hpp
*
* 任意の型が前置・後置インクリメント演算子をサポートしているかを調べるメタ関数を提供するヘッダ。
*/


#include <type_traits>

#include <meevax/debug/debug.hpp>


namespace meevax::type_traits
{
  /**
  * 前置インクリメント可能かを検査するメタ関数。
  *
  * 現時点では、このヘッダは `std::false_type` から派生するものしか提供しない。
  * 本当はスカラ型や一般クラスについてもメタ関数で検査して、`std::true_type` から
  * 派生出来るようにしておかなければならないのだが、今は必要な状況にないので後回し。
  *
  * 列挙型（スコープ付き列挙型含む）について、`std::true_type` から派生する特殊化を
  * 名前空間 `meevax::type_traits` 中に明示的に定義しておくことで、
  * ヘッダファイル `meevax/type_traits/is_supports_increment_operators.hpp`
  * 中で定義される 列挙型用の型安全グローバルインクリメント演算子オーバーロードを有効化出来る。
  */
  template <typename T>
  class is_supports_prefix_increment_operators
    : public std::false_type
  {};

  /**
  * 後置インクリメント可能かを検査するメタ関数。
  *
  * 現時点では、このヘッダは `std::false_type` から派生するものしか提供しない。
  * 本当はスカラ型や一般クラスについてもメタ関数で検査して、`std::true_type` から
  * 派生出来るようにしておかなければならないのだが、今は必要な状況にないので後回し。
  *
  * 列挙型（スコープ付き列挙型含む）について、`std::true_type` から派生する特殊化を
  * 明示的に定義しておくことで、ヘッダファイル `meevax/type_traits/is_supports_increment_operators.hpp`
  * 中で定義される 列挙型用の型安全グローバルインクリメント演算子オーバーロードを有効化出来る。
  */
  template <typename T>
  class is_supports_postfix_increment_operators
    : public std::false_type
  {};

  /**
  * 推論補助用のクラス。なんだかんだでユーザ側からアクセス可能なのが気に食わない。
  */
  class is_supports_increment_operators_
  {
  public:
    /**
    * decltype による推論で返り値型を引き出すことを目的としたメタ関数。
    * 具体性が高いため、もう片方のオーバーロードよりもこちらが優先される。
    */
    template <typename U
    #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
    , typename = typename std::enable_if<
                            meevax::type_traits::is_supports_prefix_increment_operators<U>::value
                          >::type
    , typename = typename std::enable_if<
                            meevax::type_traits::is_supports_postfix_increment_operators<U>::value
                          >::type
    #endif // DOXYGEN_TEMPLATE_SFINAE_CONCEALER
    >
    static constexpr std::true_type check(U&&) noexcept;

    /**
    * decltype による推論で返り値型を引き出すことを目的としたメタ関数。
    * もう片方のオーバーロードの推論に失敗した型はこちらに該当する。
    */
    template <typename U>
    static constexpr std::false_type check(...) noexcept;
  };

  /**
  * 前置・後置インクリメント可能かを検査するメタ関数。一応こちらが本体。
  * 推論補助クラスを利用して `std::true_type` または `std::false_type` から派生する。
  */
  template <typename T>
  class is_supports_increment_operators
    : public decltype(is_supports_increment_operators_::check(std::declval<T>()))
  {};

#ifndef NDEBUG
  /**
  * 静的テスト用のダミークラスに対する特殊化定義。前置インクリメント。
  *
  * ドキュメント化するべきものではないが、
  * 忘れてしまった後で絶対に謎の存在になって未来の自分を困惑させるのでドキュメント化。
  */
  template <>
  class is_supports_prefix_increment_operators<meevax::debug::dummy_enum>
    : public std::true_type
  {};

  /**
  * 静的テスト用のダミークラスに対する特殊化定義。後置インクリメント。
  *
  * ドキュメント化するべきものではないが、
  * 忘れてしまった後で絶対に謎の存在になって未来の自分を困惑させるのでドキュメント化。
  */
  template <>
  class is_supports_postfix_increment_operators<meevax::debug::dummy_enum>
    : public std::true_type
  {};

  static_assert(
    meevax::type_traits::is_supports_increment_operators<
      meevax::debug::dummy_enum
    >::value == true
  );
#endif // NDEBUG
} // namespace meevax::type_traits


namespace // {annonymous}
{
  /**
  * 前置インクリメント可能のコンセプトを満たす列挙型に対する前置インクリメント演算子オーバーロード。
  *
  * あくまで単純に型安全なインクリメントを提供するだけであって、オーバーフローに対する検査は行われない。
  * というか、範囲外の値からのキャストが成功するかもわからない。
  * つまり、より上位のコンセプティブな操作の道具として使われることを想定した演算。
  *
  * @param enum_class
  *
  * @return 前置インクリメントされたオブジェクトへの参照。
  */
  template <typename EnumClass
  #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  , typename = typename std::enable_if<
                          meevax::type_traits::is_supports_prefix_increment_operators<EnumClass>::value
                        >::type
  , typename = typename std::enable_if<
                          std::is_enum<EnumClass>::value
                        >::type
  #endif // DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  >
  constexpr decltype(auto) operator++(EnumClass&& enum_class)
  {
    return enum_class = static_cast<EnumClass>(static_cast<typename std::underlying_type<EnumClass>::type>(enum_class) + 1);
  }

  /**
  * 後置インクリメント可能のコンセプトを満たす列挙型に対する後置インクリメント演算子オーバーロード。
  *
  * あくまで単純に型安全なインクリメントを提供するだけであって、オーバーフローに対する検査は行われない。
  * というか、範囲外の値からのキャストが成功するかもわからない。
  * つまり、より上位のコンセプティブな操作の道具として使われることを想定した演算。
  *
  * @param enum_class
  *
  * @return 前置インクリメントされたオブジェクトへの参照。
  */
  template <typename EnumClass
  #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  , typename = typename std::enable_if<
                          meevax::type_traits::is_supports_postfix_increment_operators<EnumClass>::value
                        >::type
  , typename = typename std::enable_if<
                          std::is_enum<EnumClass>::value
                        >::type
  #endif // DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  >
  constexpr auto operator++(EnumClass&& enum_class, int)
  {
    const auto buffer {enum_class};
    enum_class = static_cast<EnumClass>(static_cast<typename std::underlying_type<EnumClass>::type>(enum_class) + 1);
    return buffer;
  }
} // {annonymous}


static_assert(
  ++(meevax::debug::dummy_enum {meevax::debug::dummy_enum::hoge})
  == meevax::debug::dummy_enum::fuga
);

static_assert(
    (meevax::debug::dummy_enum {meevax::debug::dummy_enum::hoge})++
  == meevax::debug::dummy_enum::hoge
);


#endif

