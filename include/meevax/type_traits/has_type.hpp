#ifndef INCLUDED_MEEVAX_TYPE_TRAITS_HAS_TYPE_HPP
#define INCLUDED_MEEVAX_TYPE_TRAITS_HAS_TYPE_HPP


#include <type_traits>
#include <utility>


namespace meevax {


#define implement_has_type(token) \
class has_##token##_                                                            \
{                                                                               \
public:                                                                         \
  template <typename T, typename = typename T::token>                           \
  static constexpr decltype(auto) check(T&&) noexcept                           \
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


implement_has_type(allocator_type);
implement_has_type(iterator);
implement_has_type(const_iterator);
implement_has_type(value_type);
implement_has_type(size_type);


#undef inplement_has_type


} // namespace meevax


#ifndef DEBUG
#include <vector>
static_assert(meevax::has_allocator_type<std::vector<int>>::value, "");
static_assert(meevax::has_iterator<      std::vector<int>>::value, "");
static_assert(meevax::has_const_iterator<std::vector<int>>::value, "");
static_assert(meevax::has_value_type<    std::vector<int>>::value, "");
static_assert(meevax::has_size_type<     std::vector<int>>::value, "");
#endif


#endif

