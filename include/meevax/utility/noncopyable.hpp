#ifndef INCLUDED_MEEVAX_UTILITY_NONCOPYABLE_HPP
#define INCLUDED_MEEVAX_UTILITY_NONCOPYABLE_HPP


/**
* @file noncopyable.hpp
*
* CRTP 版 `noncopyable` を提供するヘッダ。Boost に無かったので作った。
*/


#include <utility>


namespace meevax::utility
{
  /**
  * Boost にはない CRTP 版 `noncopyable` が欲しくて作った。単純なので説明することがない。
  */
  template <typename T>
  class noncopyable
  {
  public:
    /// デフォルトコピーコンストラクタ。
    noncopyable(const noncopyable&) = delete;

    /// 代入演算子オーバーロードの削除。
    T& operator=(const T&) = delete;

  protected:
    /// デフォルトコンストラクタ。
    constexpr noncopyable() = default;

    /// デフォルトデストラクタ。
    virtual ~noncopyable() = default;
  };
} // namespace meevax::utility


#endif // INCLUDED_MEEVAX_UTILITY_NONCOPYABLE_HPP

