#ifndef INCLUDED_MEEVAX_NUMERIC_IOTA_HPP
#define INCLUDED_MEEVAX_NUMERIC_IOTA_HPP


/**
* @file iota.hpp
*
* イオタ関数を提供するヘッダ。イオタという名前は APL 言語由来らしい。
*/


#include <array>
#include <iterator>
#include <numeric>
#include <vector>

#include <meevax/type_traits/type_traits.hpp>
#include <meevax/utility/utility.hpp>


#ifndef __cpp_nested_namespace_definitions
namespace meevax { namespace numeric
#else
namespace meevax::numeric
#endif // __cpp_nested_namespace_definitions
{
  /**
  * 標準コンテナによるシーケンスを返す関数。ランタイム伸長可能な動的配列型を返す。
  *
  * 返されるコンテナは to - from + 1 のサイズぴったりにリサイズされていることを保証する。
  * 返り値型は標準コンテナの右辺値型。
  * 正直微妙。deprecated ではないが、将来的に削除するかも。
  *
  * @param from
  * @param to
  *
  * @return StandardContainer<T>
  */
  template <template <typename...> typename StandardContainer, typename T
  #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  , typename = typename std::enable_if<
                          meevax::type_traits::is_standard_container<
                            StandardContainer<typename std::decay<T>::type>
                          >::value
                        >::type
  #endif // DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  >
  auto iota(T&& from, T&& to)
  {
    StandardContainer<T> v {};
    v.resize(static_cast<typename StandardContainer<T>::size_type>(to - from + 1));
    std::iota(std::begin(v), std::end(v), from);
    return std::move(v);
  }
}
#ifndef __cpp_nested_namespace_definitions
} // namespace meevax::numeric
#endif // __cpp_nested_namespace_definitions


#endif // INCLUDED_MEEVAX_NUMERIC_IOTA_HPP

