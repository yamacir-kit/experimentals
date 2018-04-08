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
*
* トークンそのものについてのメタ関数であるため、表面をオブジェクト形式マクロで包んでいる。
* 返り値は `std::true_type` ないし `std::false_type` 型オブジェクト。
*/
#define has_type(name, ...) \
  meevax::utility::evaluate<meevax::utility::overloaded<std::true_type, std::false_type>>( \
    meevax::utility::curry( \
      meevax::utility::overload( \
        [](auto arg) constexpr -> decltype(std::declval<typename decltype(arg)::type::name>(), std::true_type {}) { return {}; }, \
        [](...) constexpr -> std::false_type { return {}; }  \
      ) \
    )(meevax::type_traits::identity<__VA_ARGS__> {}) \
  )

/**
* 第二引数に指定された型が第一引数に渡した名前のメンバ関数ないし変数を持っているかをチェックするメタ関数。
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


#endif // INCLUDED_MEEVAX_TYPE_TRAITS_INTROSPECTION_HPP

