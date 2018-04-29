#ifndef INCLUDED_MEEVAX_UTILITY_OVERLOADED_OBJECTIVES_HPP
#define INCLUDED_MEEVAX_UTILITY_OVERLOADED_OBJECTIVES_HPP


/**
* @file overloaded_objectives.hpp
*
* 簡潔に多重継承されたクラスを作成するための関数 `overload_objectives` および、
* その返り値型 `overloaded_trivial_objectives` を提供するヘッダ。
*/


#include <type_traits>
#include <utility>


#ifndef __cpp_nested_namespace_definitions
namespace meevax { namespace utility
#else
namespace meevax::utility
#endif
{
  /**
  * 前方宣言。
  */
  template <typename...>
  class overloaded_trivial_objectives;

  /**
  * 引数ひとつの場合についての特殊化。
  */
  template <typename T>
  class overloaded_trivial_objectives<T>
    : public T
  {
    using car = T;
    using cdr = void;

  public:
    template <typename T_>
    constexpr overloaded_trivial_objectives(T_&& arg)
      : car {std::forward<T_>(arg)}
    {}

    constexpr overloaded_trivial_objectives() = default;
             ~overloaded_trivial_objectives() = default;
  };

  /**
  * 引数ふたつ以上の場合についての特殊化。
  */
  template <typename T, typename U, typename... Ts>
  class overloaded_trivial_objectives<T, U, Ts...>
    : public T,
      public overloaded_trivial_objectives<U, Ts...>
  {
    using car = T;
    using cdr = overloaded_trivial_objectives<U, Ts...>;

  public:
    template <typename T_, typename... Ts_>
    constexpr overloaded_trivial_objectives(T_&& arg, Ts_&&... args)
      : car {std::forward<T_>(arg)},
        cdr {std::forward<Ts_>(args)...}
    {}

    constexpr overloaded_trivial_objectives() = default;
             ~overloaded_trivial_objectives() = default;
  };


  /**
  * 前方宣言。
  */
  template <typename...>
  class overloaded_nontrivial_objectives;

  /**
  * 引数ひとつの場合についての特殊化。
  */
  template <typename T>
  class overloaded_nontrivial_objectives<T>
    : public T
  {
    using car = T;
    using cdr = void;

  public:
    template <typename T_>
    constexpr overloaded_nontrivial_objectives(T_&& arg)
      : car {std::forward<T_>(arg)}
    {}

    constexpr overloaded_nontrivial_objectives() = default;
    virtual  ~overloaded_nontrivial_objectives() = default;
  };

  /**
  * 引数ふたつ以上の場合についての特殊化。
  */
  template <typename T, typename U, typename... Ts>
  class overloaded_nontrivial_objectives<T, U, Ts...>
    : public T,
      public overloaded_nontrivial_objectives<U, Ts...>
  {
    using car = T;
    using cdr = overloaded_nontrivial_objectives<U, Ts...>;

  public:
    template <typename T_, typename... Ts_>
    constexpr overloaded_nontrivial_objectives(T_&& arg, Ts_&&... args)
      : car {std::forward<T_>(arg)},
        cdr {std::forward<Ts_>(args)...}
    {}

    constexpr overloaded_nontrivial_objectives() = default;
    virtual  ~overloaded_nontrivial_objectives() = default;
  };


  /**
  * ヘルパ関数。
  */
  template <typename... Ts>
  constexpr auto overload_objectives(Ts&&... args)
    -> typename std::conditional<
         std::is_pod<Ts>::value...,
         overloaded_trivial_objectives<typename std::decay<Ts>::type...>,
         overloaded_nontrivial_objectives<typename std::decay<Ts>::type...>
       >::type
  {
    return {std::forward<Ts>(args)...};
  }
}
#ifndef __cpp_nested_namespace_definitions
} // namespace meevax::utility
#endif


#endif // INCLUDED_MEEVAX_UTILITY_OVERLOADED_OBJECTIVES_HPP

