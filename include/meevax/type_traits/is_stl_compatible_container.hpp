#ifndef INCLUDED_MEEVAX_TYPE_TRAITS_IS_STL_COMPATIBLE_CONTAINER_HPP
#define INCLUDED_MEEVAX_TYPE_TRAITS_IS_STL_COMPATIBLE_CONTAINER_HPP


#include <type_traits>

#include <meevax/type_traits/has_function.hpp>
#include <meevax/type_traits/has_type.hpp>


namespace meevax {


template <typename T, typename = void>
class is_stl_compatible_container
  : public std::false_type
{};

template <typename T>
class is_stl_compatible_container<T, typename std::enable_if<has_allocator_type<T>::value && has_iterator<T>::value && has_const_iterator<T>::value && has_size_type<T>::value && has_value_type<T>::value && has_begin<T>::value && has_end<T>::value && has_cbegin<T>::value && has_cend<T>::value && has_size<T>::value>::type>
  : public std::true_type
{};


} // namespace meevax


#ifndef NDEBUG
#include <vector>
static_assert(meevax::is_stl_compatible_container<std::vector<int>>::value == true);
static_assert(meevax::is_stl_compatible_container<int>::value == false);
#endif


#endif


