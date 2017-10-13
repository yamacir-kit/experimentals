#ifndef INCLUDED_MEEVAX_TYPE_TRAITS_HAS_FUNCTION_HPP
#define INCLUDED_MEEVAX_TYPE_TRAITS_HAS_FUNCTION_HPP


#include <type_traits>
#include <utility>


namespace meevax {


#define implement_has_function(token) \
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


implement_has_function(begin);
implement_has_function(end);
implement_has_function(cbegin);
implement_has_function(cend);
implement_has_function(size);


#undef implement_has_function


} // namespace meevax


#ifndef NDEBUG
#include <vector>
static_assert(meevax::has_begin<std::vector<int>>::value);
static_assert(meevax::has_end<std::vector<int>>::value);
static_assert(meevax::has_cbegin<std::vector<int>>::value);
static_assert(meevax::has_cend<std::vector<int>>::value);
static_assert(meevax::has_size<std::vector<int>>::value);
#endif


#endif

