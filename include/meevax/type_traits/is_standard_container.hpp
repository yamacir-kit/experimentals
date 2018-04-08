#ifndef INCLUDED_MEEVAX_TYPE_TRAITS_IS_STANDARD_CONTAINER_HPP
#define INCLUDED_MEEVAX_TYPE_TRAITS_IS_STANDARD_CONTAINER_HPP


/**
* @file is_standard_container.hpp
*
* 標準コンテナであるか否かをチェックするメタ関数を提供するヘッダ。
*/


#include <type_traits>

#include <meevax/type_traits/introspection.hpp>
#include <meevax/type_traits/is_equality_comparable.hpp>


namespace meevax::type_traits
{
  /**
  * 標準コンテナであるか否かをチェックするメタ関数。こちらは `std::false_type` を返す方。
  *
  * @todo
  *   なんか気に入らない。
  */
  template <typename NonStandardContainer
  #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  , typename = void
  #endif // DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  >
  class is_standard_container
    : public std::false_type
  {};

  MEEVAX_TYPE_TRAITS_INTROSPECTION_DEFINE_HAS_TYPE(      reference);
  MEEVAX_TYPE_TRAITS_INTROSPECTION_DEFINE_HAS_TYPE(const_reference);
  MEEVAX_TYPE_TRAITS_INTROSPECTION_DEFINE_HAS_TYPE(      iterator);
  MEEVAX_TYPE_TRAITS_INTROSPECTION_DEFINE_HAS_TYPE(const_iterator);
  MEEVAX_TYPE_TRAITS_INTROSPECTION_DEFINE_HAS_TYPE(     value_type);
  MEEVAX_TYPE_TRAITS_INTROSPECTION_DEFINE_HAS_TYPE(difference_type);
  MEEVAX_TYPE_TRAITS_INTROSPECTION_DEFINE_HAS_TYPE(      size_type);

  MEEVAX_TYPE_TRAITS_INTROSPECTION_DEFINE_HAS_FUNCTION( begin);
  MEEVAX_TYPE_TRAITS_INTROSPECTION_DEFINE_HAS_FUNCTION(cbegin);
  MEEVAX_TYPE_TRAITS_INTROSPECTION_DEFINE_HAS_FUNCTION( end);
  MEEVAX_TYPE_TRAITS_INTROSPECTION_DEFINE_HAS_FUNCTION(cend);
  MEEVAX_TYPE_TRAITS_INTROSPECTION_DEFINE_HAS_FUNCTION(    size);
  MEEVAX_TYPE_TRAITS_INTROSPECTION_DEFINE_HAS_FUNCTION(max_size);
  MEEVAX_TYPE_TRAITS_INTROSPECTION_DEFINE_HAS_FUNCTION(empty);

  /**
  * 標準コンテナであるか否かをチェックするメタ関数。こちらは `std::true_type` を返す方。
  * 本気で厳密には作ってないので微妙に役に立たない奴。
  *
  * @todo
  *   なんか気に入らない。
  */
  template <typename StandardContainer>
  class is_standard_container<StandardContainer
  #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  , typename std::enable_if<
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
                 meevax::type_traits::has_cbegin<StandardContainer>,
                 meevax::type_traits::has_end<StandardContainer>,
                 meevax::type_traits::has_cend<StandardContainer>,
                 meevax::type_traits::is_equality_comparable<StandardContainer>,
                 std::is_swappable<StandardContainer>,
                 meevax::type_traits::has_size<StandardContainer>,
                 meevax::type_traits::has_max_size<StandardContainer>,
                 meevax::type_traits::has_empty<StandardContainer>
               >::value
             >::type
  #endif // DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  >
    : public std::true_type
  {};
}; // namespace meevax::type_traits


#endif // INCLUDED_MEEVAX_TYPE_TRAITS_IS_STANDARD_CONTAINER_HPP

