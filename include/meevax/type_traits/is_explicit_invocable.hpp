#ifndef INCLUDED_MEEVAX_TYPE_TRATIS_IS_EXPLICIT_INVOCABLE_HPP
#define INCLUDED_MEEVAX_TYPE_TRATIS_IS_EXPLICIT_INVOCABLE_HPP


#include <type_traits>

#include <meevax/utility/utility.hpp>


#ifndef __cpp_nested_namespace_definitions
namespace meevax { namespace type_traits
#else
namespace meevax::type_traits
#endif
{
  class is_explicit_invocable_
  {
  public:
    template <typename Function, typename... Ts>
    static constexpr auto check(Function&& function, Ts&&... args)
      -> decltype(meevax::utility::explicit_function<Function> {}(std::forward<Ts>(args)...), std::true_type {});

    template <typename...>
    static constexpr auto check(...) noexcept
      -> std::false_type;
  };

  template <typename T, typename... Ts>
  class is_explicit_invocable
    : public decltype(meevax::type_traits::is_explicit_invocable_::check<T>(
               std::forward<T>(std::declval<T>()), std::forward<Ts>(std::declval<Ts>())...
             ))
  {};
}
#ifndef __cpp_nested_namespace_definitions
} // namespace meevax::type_traits
#endif


#endif // INCLUDED_MEEVAX_TYPE_TRATIS_IS_EXPLICIT_INVOCABLE_HPP


