#ifndef INCLUDED_MEEVAX_CONCEPTS_IS_STANDARD_CONTAINER_HPP
#define INCLUDED_MEEVAX_CONCEPTS_IS_STANDARD_CONTAINER_HPP


#include <type_traits>

#include <meevax/concepts/is_equality_comparable.hpp>
#include <meevax/type_traits/has_function.hpp>
#include <meevax/type_traits/has_type.hpp>


#if __cplusplus < 201703L
namespace meevax {
namespace concepts {
#else
namespace meevax::concepts {
#endif


// http://en.cppreference.com/w/cpp/concept/Container

template <typename NonStandardContainer, typename = void>
class is_standard_container
  : public std::false_type
{};

template <typename StandardContainer>
class is_standard_container<StandardContainer,
                            typename std::enable_if<
                              std::conjunction<
                                meevax::type_traits::has_value_type<StandardContainer>,
                                meevax::type_traits::has_reference<StandardContainer>,
                                meevax::type_traits::has_const_reference<StandardContainer>,
                                meevax::type_traits::has_iterator<StandardContainer>,
                                meevax::type_traits::has_const_iterator<StandardContainer>,
                                meevax::type_traits::has_difference_type<StandardContainer>,
                                meevax::type_traits::has_size_type<StandardContainer>,
                                std::is_default_constructible<StandardContainer>,
                                std::is_copy_constructible<StandardContainer>,
                                std::is_assignable<StandardContainer, StandardContainer>,
                                std::is_destructible<StandardContainer>,
                                meevax::type_traits::has_begin<StandardContainer>,
                                meevax::type_traits::has_end<StandardContainer>,
                                meevax::type_traits::has_cbegin<StandardContainer>,
                                meevax::type_traits::has_cend<StandardContainer>,
                                meevax::concepts::is_equality_comparable<StandardContainer>,
                                std::is_swappable<StandardContainer>,
                                meevax::type_traits::has_size<StandardContainer>,
                                meevax::type_traits::has_max_size<StandardContainer>,
                                meevax::type_traits::has_empty<StandardContainer>
                              >::value
                            >::type>
  : public std::true_type
{};


#if __cplusplus < 201703L
} // namespace concepts
} // namespace meevax
#else
} // namespace meevax::concepts
#endif


#endif

