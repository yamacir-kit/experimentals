#ifndef INCLUDED_MEEVAX_TYPE_TRAITS_INTROSPECTION_HPP
#define INCLUDED_MEEVAX_TYPE_TRAITS_INTROSPECTION_HPP


/**
* @file introspection.hpp
*
* 使用状況が限定的だが、いわゆる汎用 `has_type` マクロメタ関数を提供するヘッダ。
*/


#include <type_traits>

#include <meevax/debug/debug.hpp>
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
(false ? meevax::utility::overload( \
           [](auto arg) constexpr -> decltype(std::declval<typename decltype(arg)::type::name>(), std::true_type {}) { return {}; }, \
           [](...) constexpr -> std::false_type { return {}; }  \
         )(meevax::type_traits::identity<__VA_ARGS__> {}) \
       : meevax::utility::overloaded<std::true_type, std::false_type> {})

static_assert(has_type(value_type, meevax::debug::dummy_type).value);


#endif // INCLUDED_MEEVAX_TYPE_TRAITS_INTROSPECTION_HPP

