#ifndef INCLUDED_MEEVAX_TYPE_TRAITS_HAS_FUNCTION_HPP
#define INCLUDED_MEEVAX_TYPE_TRAITS_HAS_FUNCTION_HPP


#include <type_traits>
#include <utility>


namespace meevax {


#define implement_has_function(token) \
class has_##token##_                                                            \
{                                                                               \
  template <typename T>                                                         \
  static constexpr auto check(T& structure) noexcept                            \
    -> decltype(structure.token(), std::true_type {})                           \
  {                                                                             \
    return std::true_type {};                                                   \
  }                                                                             \
                                                                                \
  template <typename T>                                                         \
  static constexpr decltype(auto) check(...) noexcept                           \
  {                                                                             \
    return std::false_type {};                                                  \
  }                                                                             \
};                                                                              \
                                                                                \
template <typename T>                                                           \
class has_##token                                                               \
  : public decltype(has##token##_::check<T>(std::declval<T>()))                 \
{};


implement_has_function(begin);
implement_has_function(end);
implement_has_function(cbegin);
implement_has_function(cend);
implement_has_function(size);


#undef implement_has_function


} // namespace meevax


#endif

