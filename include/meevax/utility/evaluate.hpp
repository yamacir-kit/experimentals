#ifndef INCLUDED_MEEVAX_UTILITY_EVALUATE_HPP
#define INCLUDED_MEEVAX_UTILITY_EVALUATE_HPP


/**
* @file evaluate.hpp
*
* 特定の文脈でオペランドを評価するための道具を提供するヘッダ。
*/


#include <utility>

#include <meevax/utility/overload.hpp>


namespace meevax::utility
{
  /**
  * SFINAE の文脈で constexpr ラムダ式を評価するための道具（規格で禁止されているため普通には書けない）。
  *
  * ## Usage
  *   使い方がどうもまどろっこしいが、基本的に `meevax::utility::overload` と一緒に使用する。
  *
  *   条件演算子の第二、第三オペランドの返り値型が同じでなければならないという事実を利用する。
  *   第三オペランドの返り値型が第三オペランドだけでは推論できない状況を意図的に作り出し、
  *   そのヒントとして第二オペランドの返り値を求めさせることが目的である。
  *   つまり、本関数の返り値型は第二オペランドの評価結果に依存する。
  *
  * ## References
  *   本関数は、基本的には以下のウェブページを参考にしつつパーツとして整理したもの。
  *   [https://qiita.com/_EnumHack/items/ee2141ad47915c55d9cb#c14-generic-lambda%E3%83%91%E3%83%AF%E3%83%BC%E3%81%A7%E6%B1%8E%E7%94%A8%E7%9A%84-has_xxx--%E8%BF%BD%E8%A8%982017-9-03]
  *
  * @param function
  *   評価したい多重継承 constexpr ラムダ式。
  *
  * @return
  *   `std::true_type` または `std::false_type` 型オブジェクト。
  *
  * @todo
  *   説明がクソ。
  */
  template <typename Function>
  constexpr auto evaluate(Function&& function)
  {
    return false ? function() : meevax::utility::overloaded<std::true_type, std::false_type> {};
  }

  /**
  * 評価したい constexpr ラムダの返り値型が `std::true_type` または `std::false_type` 型
  * ではない場合のための関数。
  * キレイに書く方法が思いつかないためとりあえずの実装。
  *
  * だいたい次の要領で書く。`namespace meevax::utility` を省略している。
  * `evaluate<overloaded<return_types>>(curry(overload(lambdas...)(args...)))`
  *
  * @param function
  *   評価したい多重継承 constexpr ラムダ式。
  *
  * @return
  *   テンプレートパラメータ `ReturnTypes` に渡した `meevax::utility::overloaded<types...>` の `types...` のいずれか。
  *
  * @todo
  *   説明がクソ。
  */
  template <typename ReturnTypes, typename Function>
  constexpr auto evaluate(Function&& function)
  {
    return false ? function() : ReturnTypes {};
  }
};


#endif // INCLUDED_MEEVAX_UTILITY_EVALUATE_HPP

