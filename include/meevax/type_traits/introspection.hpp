#ifndef INCLUDED_MEEVAX_TYPE_TRAITS_INTROSPECTION_HPP
#define INCLUDED_MEEVAX_TYPE_TRAITS_INTROSPECTION_HPP


/**
* @file introspection.hpp
*
* いわゆる汎用 `has_xxx` マクロメタ関数を提供するヘッダ。
*/


#include <type_traits>

#include <meevax/utility/utility.hpp>


namespace meevax::type_traits
{
  template <typename T>
  class identity
  {
  public:
    using type = T;
  };
};

/**
* 第二引数に指定された型が第一引数に渡した名前のメンバ型を持っているかをチェックするメタ関数。
* 汎用だが SFINAE の文脈で使用することは出来ない。
*
* トークンそのものについてのメタ関数であるため、表面をオブジェクト形式マクロで包んでいる。
* 返り値は `std::true_type` ないし `std::false_type` 型オブジェクト。
*/
#define has_type(name, ...) \
meevax::utility::evaluate( \
  meevax::utility::curry( \
    meevax::utility::overload( \
      [](auto arg) constexpr -> decltype(std::declval<typename decltype(arg)::type::name>(), std::true_type {}) { return {}; }, \
      [](...) constexpr -> std::false_type { return {}; }  \
    ) \
  )(meevax::type_traits::identity<__VA_ARGS__> {}) \
)

/**
* 第二引数に指定された型が第一引数に渡した名前のメンバ関数ないし変数を持っているかをチェックするメタ関数。
* 汎用だが SFINAE の文脈で使用することは出来ない。
*
* トークンそのものについてのメタ関数であるため、表面をオブジェクト形式マクロで包んでいる。
* 返り値は `std::true_type` ないし `std::false_type` 型オブジェクト。
*/
#define has_member(name, ...) \
meevax::utility::evaluate( \
  meevax::utility::curry( \
    meevax::utility::overload( \
      [](auto arg) constexpr -> decltype(&decltype(arg)::type::name, std::true_type {}) { return {}; }, \
      [](...) constexpr -> std::false_type { return {}; }  \
    ) \
  )(meevax::type_traits::identity<__VA_ARGS__> {}) \
)

/**
* テンプレートパラメータに渡した型がある名前のメンバ型を持っているかをチェックするメタ関数、
* を量産するためのオブジェクト形式マクロ。
* 事前定義が必要になってしまうが、SFINAE の文脈でも使用できる。
*/
#define MEEVAX_TYPE_TRAITS_INTROSPECTION_DEFINE_HAS_TYPE(token) \
class has_##token##_ \
{ \
public: \
  template <typename T, typename = typename T::token> \
  static constexpr auto check(T&&) noexcept \
    -> std::true_type; \
  template <typename T> \
  static constexpr auto check(...) noexcept \
    -> std::false_type; \
}; \
template <typename T> \
class has_##token \
  : public decltype(has_##token##_::check(std::declval<T>())) \
{};

/**
* テンプレートパラメータに渡した型がある名前のメンバ関数を持っているかをチェックするメタ関数、
* を量産するためのオブジェクト形式マクロ。
* 事前定義が必要になってしまうが、SFINAE の文脈でも使用できる。
*/
#define MEEVAX_TYPE_TRAITS_INTROSPECTION_DEFINE_HAS_FUNCTION(token) \
class has_##token##_ \
{ \
public: \
  template <typename T> \
  static constexpr auto check(T&& hoge) noexcept \
    -> decltype(hoge.token(), std::true_type {}); \
  template <typename T> \
  static constexpr auto check(...) noexcept \
    -> std::false_type; \
}; \
template <typename T> \
class has_##token \
  : public decltype(has_##token##_::check(std::declval<T>())) \
{};


#endif // INCLUDED_MEEVAX_TYPE_TRAITS_INTROSPECTION_HPP

