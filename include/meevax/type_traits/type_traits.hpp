#ifndef INCLUDED_MEEVAX_TYPE_TRAITS_TYPE_TRAITS_HPP
#define INCLUDED_MEEVAX_TYPE_TRAITS_TYPE_TRAITS_HPP


/**
* @file type_traits.hpp
*
* 名前空間 `meevax::type_traits` を定義するヘッダ群の一括インクルードを提供するヘッダ。
* また、標準ライブラリおよび Boost ライブラリの `type_traits` ヘッダもインクルードする。
*/


#include <boost/type_traits.hpp>

#include <meevax/type_traits/is_char_type.hpp>
#include <meevax/type_traits/is_scoped_bitmasks.hpp>


/**
* 標準ライブラリ `type_traits` の拡張メタ関数群を含む名前空間。
* 主にテンプレートメタプログラミングに用いる小道具が置かれる。
*/
namespace meevax::type_traits {};


#endif // INCLUDED_MEEVAX_TYPE_TRAITS_TYPE_TRAITS_HPP

