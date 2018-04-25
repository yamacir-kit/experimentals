#ifndef INCLUDED_MEEVAX_TYPE_TRAITS_IS_STANDARD_CONTAINER_HPP
#define INCLUDED_MEEVAX_TYPE_TRAITS_IS_STANDARD_CONTAINER_HPP


/**
* @file is_standard_container.hpp
*
* 標準コンテナであるか否かをチェックするメタ関数を提供するヘッダ。
*/


#include <type_traits>

#include <boost/tti/has_member_function.hpp>
#include <boost/tti/has_type.hpp>

#ifndef __cpp_lib_logical_traits
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#endif

#include <meevax/type_traits/is_equality_comparable.hpp>


#ifndef __cpp_nested_namespace_definitions
namespace meevax { namespace type_traits
#else
namespace meevax::type_traits
#endif
{
  BOOST_TTI_HAS_TYPE(      reference);
  BOOST_TTI_HAS_TYPE(const_reference);

  BOOST_TTI_HAS_TYPE(      iterator);
  BOOST_TTI_HAS_TYPE(const_iterator);

  BOOST_TTI_HAS_TYPE(     value_type);
  BOOST_TTI_HAS_TYPE(difference_type);
  BOOST_TTI_HAS_TYPE(      size_type);

  BOOST_TTI_HAS_MEMBER_FUNCTION( begin);
  BOOST_TTI_HAS_MEMBER_FUNCTION(cbegin);

  BOOST_TTI_HAS_MEMBER_FUNCTION( end);
  BOOST_TTI_HAS_MEMBER_FUNCTION(cend);

  BOOST_TTI_HAS_MEMBER_FUNCTION(    size);
  BOOST_TTI_HAS_MEMBER_FUNCTION(max_size);

  BOOST_TTI_HAS_MEMBER_FUNCTION(empty);

  /**
  * 標準コンテナであるか否かをチェックするメタ関数。こちらは `std::true_type` を返す方。
  * 本気で厳密には作ってないので微妙に役に立たない奴。
  */
  template <typename T>
  class is_standard_container
    #ifndef __cpp_lib_logical_traits
    : public boost::mpl::and_<
    #else
    : public std::conjunction<
    #endif
        meevax::type_traits::has_type_value_type<T>,
        meevax::type_traits::has_type_reference<T>,
        meevax::type_traits::has_type_const_reference<T>,
        meevax::type_traits::has_type_iterator<T>,
        meevax::type_traits::has_type_const_iterator<T>,
        meevax::type_traits::has_type_difference_type<T>,
        meevax::type_traits::has_type_size_type<T>,
        std::is_default_constructible<T>,
        std::is_copy_constructible<T>,
        std::is_assignable<T, T>,
        std::is_destructible<T>,
        #ifndef __cpp_lib_logical_traits
        boost::mpl::or_<
        #else
        std::disjunction<
        #endif
          meevax::type_traits::has_member_function_begin<
            typename T::iterator (T::*)(void)
          >,
          meevax::type_traits::has_member_function_begin<
            typename T::const_iterator (T::*)(void) const
          >
        >,
        meevax::type_traits::has_member_function_cbegin<
          typename T::const_iterator (T::*)(void) const
        >,
        #ifndef __cpp_lib_logical_traits
        boost::mpl::or_<
        #else
        std::disjunction<
        #endif
          meevax::type_traits::has_member_function_end<
            typename T::iterator (T::*)(void)
          >,
          meevax::type_traits::has_member_function_end<
            typename T::const_iterator (T::*)(void) const
          >
        >,
        meevax::type_traits::has_member_function_cend<
          typename T::const_iterator (T::*)(void) const
        >,
        meevax::type_traits::is_equality_comparable<T>,
        std::is_swappable<T>,
        meevax::type_traits::has_member_function_size<
          typename T::size_type (T::*)(void) const
        >,
        meevax::type_traits::has_member_function_max_size<
          typename T::size_type (T::*)(void) const
        >,
        meevax::type_traits::has_member_function_empty<
          bool (T::*)(void) const
        >
      >
  {};
}; // namespace meevax::type_traits


#endif // INCLUDED_MEEVAX_TYPE_TRAITS_IS_STANDARD_CONTAINER_HPP

