#ifndef INCLUDED_MEEVAX_UTILITY_OVERLOADED_OPERATIONS_HPP
#define INCLUDED_MEEVAX_UTILITY_OVERLOADED_OPERATIONS_HPP


/**
* @file overloaded_operations.hpp
*
* 簡潔に多重継承されたクラスを作成するための関数 `overload_operations` および、
* その返り値型 `overloaded_operations` を提供するヘッダ。
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
  class overloaded_operations;

  /**
  * 引数ひとつの場合についての特殊化。
  */
  template <typename T>
  class overloaded_operations<T>
    : public T
  {
    using car = T;
    using cdr = void;

  public:
    template <typename T_>
    constexpr overloaded_operations(T_&& arg)
      : car {std::forward<T_>(arg)}
    {}

    constexpr overloaded_operations() = default;
    virtual  ~overloaded_operations() = default;

    using car::operator();
  };

  /**
  * 引数ふたつ以上の場合についての特殊化。
  */
  template <typename T, typename U, typename... Ts>
  class overloaded_operations<T, U, Ts...>
    : public T,
      public overloaded_operations<U, Ts...>
  {
    using car = T;
    using cdr = overloaded_operations<U, Ts...>;

  public:
    template <typename T_, typename... Ts_>
    constexpr overloaded_operations(T_&& arg, Ts_&&... args)
      : car {std::forward<T_>(arg)},
        cdr {std::forward<Ts_>(args)...}
    {}

    constexpr overloaded_operations() = default;
    virtual  ~overloaded_operations() = default;

    using car::operator();
    using cdr::operator();
  };

  /**
  * ヘルパ関数。
  */
  template <typename... Ts>
  constexpr auto overload_operations(Ts&&... args)
    -> overloaded_operations<typename std::decay<Ts>::type...>
  {
    return {std::forward<Ts>(args)...};
  }
}
#ifndef __cpp_nested_namespace_definitions
} // namespace meevax::utility
#endif


#endif // INCLUDED_MEEVAX_UTILITY_OVERLOADED_OPERATIONS_HPP

