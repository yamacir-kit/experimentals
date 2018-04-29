#ifndef INCLUDED_MEEVAX_TYPE_TRATIS_IS_IMPLICIT_INVOCABLE_HPP
#define INCLUDED_MEEVAX_TYPE_TRATIS_IS_IMPLICIT_INVOCABLE_HPP


#include <type_traits>
#include <utility>


#ifndef __cpp_nested_namespace_definitions
namespace meevax { namespace type_traits
#else
namespace meevax::type_traits
#endif
{
  class is_implicit_invocable_
  {
  public:
    template <typename T, typename... Ts>
    static constexpr auto check(T&& function, Ts&&... args)
      -> decltype(function(std::forward<Ts>(args)...), std::true_type {});

    template <typename...>
    static constexpr auto check(...) noexcept
      -> std::false_type;
  };

  template <typename T, typename... Ts>
  class is_implicit_invocable
    : public decltype(meevax::type_traits::is_implicit_invocable_::check<T>(
               std::forward<T>(std::declval<T>()), std::forward<Ts>(std::declval<Ts>())...
             ))
  {};
}
#ifndef __cpp_nested_namespace_definitions
} // namespace meevax::type_traits
#endif


#endif // INCLUDED_MEEVAX_TYPE_TRATIS_IS_IMPLICIT_INVOCABLE_HPP

