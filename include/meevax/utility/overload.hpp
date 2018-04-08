#ifndef INCLUDED_MEEVAX_UTILITY_OVERLOAD_HPP
#define INCLUDED_MEEVAX_UTILITY_OVERLOAD_HPP


/**
* @file overload.hpp
*
* 簡潔に多重継承されたクラスを作成するための関数 `overload` および、
* その返り値型 `overloaded` を提供するヘッダ。
*/


#include <tuple>
#include <utility>

#include <meevax/type_traits/type_traits.hpp>


namespace meevax::utility
{
  /**
  * 前方宣言。
  */
  template <typename... Ts>
  class overloaded;

  /**
  * 引数ひとつの場合についての特殊化。
  */
  template <typename T>
  class overloaded<T>
    : public T
  {
    using car = T;
    using cdr = void;

  public:
    constexpr overloaded(T&& data)
      : car {std::forward<T>(data)}
    {}

    constexpr overloaded() = default;
    // virtual  ~overloaded() = default;

    using car::operator();
  };

  /**
  * 引数ふたつ以上の場合についての特殊化。
  */
  template <typename T, typename U, typename... Ts>
  class overloaded<T, U, Ts...>
    : public T,
      public overloaded<U, Ts...>
  {
    using car = T;
    using cdr = overloaded<U, Ts...>;

  public:
    constexpr overloaded(T&& data, U&& arg, Ts&&... args)
      : car {std::forward<T>(data)},
        cdr {std::forward<U>(arg), std::forward<Ts>(args)...}
    {}

    constexpr overloaded() = default;
    // virtual  ~overloaded() = default;

    using car::operator();
    using cdr::operator();
  };

  /**
  * ヘルパ関数。
  */
  template <typename... Ts>
  constexpr auto overload(Ts&&... args)
    -> overloaded<typename std::decay<Ts>::type...>
  {
    return {std::forward<Ts>(args)...};
  }
}; // namespace meevax::utility


#endif // INCLUDED_MEEVAX_UTILITY_OVERLOAD_HPP

