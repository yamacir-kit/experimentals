#ifndef INCLUDED_MEEVAX_UTILITY_UTILITY_HPP
#define INCLUDED_MEEVAX_UTILITY_UTILITY_HPP


/**
* @file utility.hpp
*
* 名前空間 `meevax::utility` を定義するヘッダ群の一括インクルードを提供するヘッダ。
* また、標準ライブラリおよび Boost ライブラリの `utility` ヘッダもインクルードする。
*/


#include <boost/utility.hpp>

#include <meevax/utility/noncopyable.hpp>
#include <meevax/utility/unmovable.hpp>


/**
* あまりに基本的すぎて置き場所に困ったものが置かれる名前空間。
* C++ 文法にはないセマンティクスを実現する単純クラス群の置き場と考えて概ね間違いない。
*/
namespace meevax::utility {}


#endif // INCLUDED_MEEVAX_UTILITY_UTILITY_HPP

