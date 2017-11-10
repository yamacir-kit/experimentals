#ifndef INCLUDED_MEEVAX_CONCEPTS_IS_EQUALITY_COMPARABLE_HPP
#define INCLUDED_MEEVAX_CONCEPTS_IS_EQUALITY_COMPARABLE_HPP


#include <type_traits>

#include <meevax/type_traits/void_type.hpp>


#if __cplusplus < 201703L
namespace meevax {
namespace concepts {
#else
namespace meevax::concepts {
#endif


template <typename NotEqualityComparable, typename = void>
class is_equality_comparable
  : public std::false_type
{};


template <typename EqualityComparable>
class is_equality_comparable<EqualityComparable,
                             meevax::type_traits::void_type<decltype(
                               std::declval<EqualityComparable&>() == std::declval<EqualityComparable&>()
                             )>>
  : public std::true_type
{};


#if __cplusplus < 201703L
} // namespace concepts
} // namespace meevax
#else
} // namespace meevax::concepts
#endif


#endif

