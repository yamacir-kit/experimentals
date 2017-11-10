#ifndef INCLUDED_MEEVAX_CONCEPTS_HAS_FUNCTION_HPP
#define INCLUDED_MEEVAX_CONCEPTS_HAS_FUNCTION_HPP


#include <type_traits>
#include <utility>


#if __cplusplus < 201703L
namespace meevax {
namespace concepts {
#else
namespace meevax::concepts {
#endif


#ifdef has_function_
static_assert(true == false, "macro \"has_function_\" has been defined outside the meevax library");
#endif

#define has_function_(token) \
class has_##token##_                                                            \
{                                                                               \
public:                                                                         \
  template <typename T>                                                         \
  static constexpr auto check(T&& structure) noexcept                           \
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
  : public decltype(has_##token##_::check<T>(std::declval<T>()))                \
{};


has_function_(begin);
has_function_(end);
has_function_(cbegin);
has_function_(cend);
has_function_(size);


#undef has_function_


#if __cplusplus < 201703L
} // namespace concepts
} // namespace meevax
#else
} // namespace meevax::concepts
#endif


#endif

