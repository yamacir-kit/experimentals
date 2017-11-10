#ifndef INCLUDED_MEEVAX_CONCEPTS_HAS_TYPE_HPP
#define INCLUDED_MEEVAX_CONCEPTS_HAS_TYPE_HPP


#include <type_traits>
#include <utility>


#if __cplusplus < 201703L
namespace meevax {
namespace concepts {
#else
namespace meevax::concepts {
#endif


#ifdef has_type_
static_assert(true == false, "macro \"has_type_\" has been defined outside the meevax library");
#endif

#define has_type_(token) \
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


has_type_(value_type);

has_type_(reference);
has_type_(const_reference);

has_type_(iterator);
has_type_(const_iterator);

has_type_(allocator_type);

has_type_(difference_type);

has_type_(size_type);


#undef has_type_


#if __cplusplus < 201703L
} // namespace concepts
} // namespace meevax
#else
} // namespace meevax::concepts
#endif


#endif

