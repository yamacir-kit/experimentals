#ifndef INCLUDED_MEEVAX_SYNTAX_SYNTAX_HPP
#define INCLUDED_MEEVAX_SYNTAX_SYNTAX_HPP

/**
* @file syntax.hpp
*
* 構文解析用のライブラリ群の一括インクルードを提供するヘッダ。
*/


#include <meevax/syntax/lexer.hpp>


/**
* 構文解析用ライブラリが収められる名前空間。
*
* 構文解析に DSL を使わないのは、DSL を使うより適当なパーサの方が
* 速くて解りやすいくらいに簡潔なコア言語にすることが言語設計における目標のひとつであるため。
* あるいは自己満足。
*/
namespace meevax::syntax {};


#endif // INCLUDED_MEEVAX_SYNTAX_SYNTAX_HPP

