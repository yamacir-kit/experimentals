#ifndef INCLUDED_MEEVAX_UTILITY_UNMOVABLE_HPP
#define INCLUDED_MEEVAX_UTILITY_UNMOVABLE_HPP


/**
* @file unmovable.hpp
*
* CRTP 版 `unmovable` を提供するヘッダ。Boost に無かったので作った。
*/


namespace meevax::utility
{
  /**
  * Boost にはない CRTP 版 `unmovable` が欲しくて作った。単純なので説明することがない。
  */
  template <typename T>
  class unmovable
  {
  public:
    /// デフォルトムーブ代入コンストラクタ。
    unmovable(unmovable&&) = delete;

    /// ムーブ代入演算子オーバーロードの削除。
    T& operator=(T&&) = delete;

  protected:
    /// デフォルトコンストラクタ。
    constexpr unmovable() = default;

    /// デフォルトデストラクタ。
    virtual ~unmovable() = default;
  };
} // namespace meevax::utility


#endif

