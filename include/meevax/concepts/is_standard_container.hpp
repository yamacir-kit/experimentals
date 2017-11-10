#ifndef INCLUDED_MEEVAX_CONCEPTS_IS_STANDARD_CONTAINER_HPP
#define INCLUDED_MEEVAX_CONCEPTS_IS_STANDARD_CONTAINER_HPP


#include <type_traits>

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

template <typename T>
class is_standard_container<T, typename std::enable_if<has_allocator_type<T>::value && has_iterator<T>::value && has_const_iterator<T>::value && has_size_type<T>::value && has_value_type<T>::value && has_begin<T>::value && has_end<T>::value && has_cbegin<T>::value && has_cend<T>::value && has_size<T>::value>::type>
  : public std::true_type
{};


#if __cplusplus < 201703L
} // namespace concepts
} // namespace meevax
#else
} // namespace meevax::concepts
#endif


#endif

