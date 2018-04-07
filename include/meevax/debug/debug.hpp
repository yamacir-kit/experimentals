#ifndef INCLUDED_MEEVAX_DEBUG_DEBUG_HPP
#define INCLUDED_MEEVAX_DEBUG_DEBUG_HPP


/**
* @file debug.hpp
*
* デバッグ用の道具を一括インクルードするためのヘッダ。
* 静的（かつ雑）なアサーションとかデバッグ用の出力関数などを提供する。
*
* このファイルを使用する側はマクロ `NDEBUG` を気にせずこのヘッダをインクルードして、
* このヘッダのコンテンツを使用して良い。
*
* マクロ `NDEBUG` を見てのコード消去などはこちら側で行う。
* 基本的には、関数が実体としてはオブジェクト形式マクロに置き換えられる程度で、
* ランタイムのオーバーヘッドがゼロになることを保証する（つもり）。
*/


#include <meevax/debug/dummy_type.hpp>

/**
* デバッグ用のクラス、関数、マクロなどが含まれる名前空間。
* プリプロセッサマクロ `NDEBUG` にゴリゴリに依存する。
*/
namespace meevax::debug {};


#endif // INCLUDED_MEEVAX_DEBUG_DEBUG_HPP

