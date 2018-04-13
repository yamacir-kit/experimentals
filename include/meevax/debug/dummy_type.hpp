#ifndef INCLUDED_MEEVAX_DEBUG_DUMMY_TYPE_HPP
#define INCLUDED_MEEVAX_DEBUG_DUMMY_TYPE_HPP


/**
* @file dummy_type.hpp
*
* デバッグ用のダミークラスの提供。
* 基本的に `static_assert` 内でのテストに掛けるためのクラス。
* 基本的に研究不足。
*/


namespace meevax::debug
{
  /**
  * ダミークラス。ユニークな名前で、他と何の依存もないクラスが必要な時のために。
  *
  * オーバーヘッドが無いため、マクロ `NDEBUG` に依存していない。
  */
  class dummy_type
  {
  public:
    using value_type = int;

  private:
    value_type data_;

    const auto& data() const noexcept
    {
      return data_;
    }
  };

  /**
  * ダミーのスコープ付き列挙型。特別なことは何もない。
  *
  * オーバーヘッドが無いため、マクロ `NDEBUG` に依存していない。
  */
  enum class dummy_enum
  {
    hoge, ///< メタ構文変数。
    fuga, ///< メタ構文変数。
    piyo, ///< メタ構文変数。
  };

  /**
  * ダミーのスコープ付き列挙型。
  * 将来的に実装予定のメタイテレータ（仮称）のためのメンバを含む。
  *
  * オーバーヘッドが無いため、マクロ `NDEBUG` に依存していない。
  */
  enum class dummy_meta_iterator
  {
    hoge, ///< メタ構文変数。
    fuga, ///< メタ構文変数。
    piyo, ///< メタ構文変数。

    size, ///< メタイテレータ用のメンバ。この位置になければ機能しない。

    min = hoge, ///< メタイテレータ用のメンバ。かなり恣意的にユーザが設定する必要がある。
    max = piyo, ///< メタイテレータ用のメンバ。かなり恣意的にユーザが設定する必要がある。

    begin = min, ///< メタイテレータ用のメンバ。これが `min` と等価になるかは場合による。
      end = max, ///< メタイテレータ用のメンバ。これが `min` と等価になるかは場合による。
  };
}; // meevax::debug


#endif // INCLUDED_MEEVAX_DEBUG_DUMMY_TYPE_HPP

