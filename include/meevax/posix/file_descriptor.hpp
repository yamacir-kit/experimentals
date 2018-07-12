#ifndef INCLUDED_MEEVAX_POSIX_FILE_DESCRIPTOR_HPP
#define INCLUDED_MEEVAX_POSIX_FILE_DESCRIPTOR_HPP


/**
* @file file_descriptor.hpp
*
* The header file which provides class for safe POSIX file descriptor control.
*/


#include <system_error>
#include <unistd.h>
#include <utility>

#include <meevax/utility/noncopyable.hpp>


namespace meevax::posix
{
  /**
  * ファイルディスクリプタの安全な管理とアクセスを提供するクラス。
  * ただの `int fd` では我慢できなくなったから作った。
  *
  * 放っておいても勝手に上手いこと閉じてくれるファイルディスクリプタがこれの目指すところ。
  * このクラスが管理するファイルディスクリプタがどのように使われるかはこのクラスが関知するところではない。
  * 具体的には、このクラスはメンバ関数`close`を持っているが`open`を持ってない。
  *
  * 最低限の機能のみを実装しているが、インタフェースは`std::unique_ptr`に倣った。
  */
  class file_descriptor
    : public meevax::utility::noncopyable<file_descriptor>
  {
  public:
    /// メタ関数のための型定義。POSIX システムの定める型、つまり `signed int`。
    using value_type = int;

  public:
    /// 標準入力のファイルディスクリプタ値。整数値の0。
    static constexpr value_type stdin  { STDIN_FILENO};

    /// 標準出力のファイルディスクリプタ値。整数値の1。
    static constexpr value_type stdout {STDOUT_FILENO};

    /// 標準エラー出力のファイルディスクリプタ値。整数値の2。
    static constexpr value_type stderr {STDERR_FILENO};

    /// 無効値。基本的に負値ならすべてエラーだが、クラス内部では負値の代表値を用いることにする。
    static constexpr value_type invalid_value {-1};

    /// このクラスが管理するファイルディスクリプタの値。private にしておくべきか、うーむ。
    value_type value;

  public:
    /**
    * コンストラクタ。
    *
    * @param fd 管理対象のファイルディスクリプタ値。
    */
    constexpr file_descriptor(value_type fd = invalid_value) noexcept
      : value {fd}
    {}

    /**
    * Move constructor.
    *
    * @param moved
    *   ムーブされてくる`meevax::posix::file_descriptor`型オブジェクト。
    *   被ムーブオブジェクトが管理していた値はこちらに移管される。
    */
    explicit file_descriptor(file_descriptor&& moved) noexcept
      : value {moved.release()}
    {}

    /// デストラクタ。管理していたファイルディスクリプタの後始末。
    ~file_descriptor() noexcept
    {
      close();
    }

    /// キャスト演算子。通常と同じくファイルディスクリプタ値に用がある際はこちらをどうぞ。
    constexpr operator value_type() const noexcept
    {
      return value;
    }

    /// 真偽値へのキャスト演算子。ファイルディスクリプタとしての有効性を示す。
    constexpr operator bool() const noexcept
    {
      return invalid_value < value;
    }

    /**
    * このクラスオブジェクトが管理していたファイルディスクリプタを開放する。
    * クローズは行わずあくまで管理下から手放すだけであるため、クローズには`file_descriptor::close`を用いること。
    *
    * @return
    *   元々保持していたファイルディスクリプタ値。
    *   無効になっているかもしれないし、まだ有効かもしれない。
    */
    value_type release() noexcept
    {
      return std::exchange(value, invalid_value);
    }

    /**
    * Close the file descriptor.
    * Execute system call `::close()` to the file descriptor under management, then set invalid value.
    *
    * @exception
    *   ファイルディスクリプタに対するクローズが失敗した時。
    */
    void close() noexcept(false)
    {
      if (stderr < value)
      {
        if (const auto status {::close(release())}; status < 0)
        {
          throw std::system_error {errno, std::system_category()};
        }
      }
    }

    /**
    * ファイルディスクリプタの交換。
    *
    * @param fd 交換対象の`meevax::posix::file_descriptor`型オブジェクト。
    *
    * @return decltype(auto)
    */
    decltype(auto) swap(file_descriptor& fd) noexcept
    {
      return std::swap(value, fd.value);
    }

    /**
    * ファイルディスクリプタの交換。こちらは標準`swap`との互換のために存在する。
    *
    * @param lhs 左辺値。
    * @param rhs 右辺値。
    *
    * @return decltype(auto)
    */
    friend decltype(auto) swap(file_descriptor& lhs, file_descriptor& rhs) noexcept
    {
      return lhs.swap(rhs);
    }

    /**
    * このクラスの管理するファイルディスクリプタを新たに設定し直す。
    * 元々管理していたファイルディスクリプタは閉じられる。
    *
    * @param new_value 新たに設定されるファイルディスクリプタ値。
    *
    * @return decltype(auto)
    */
    decltype(auto) reset(value_type new_value) noexcept
    {
      return file_descriptor(new_value).swap(*this);
    }

    /**
    * ファイルディスクリプタが端末を参照しているかの真偽値を返す。
    * エラー内容を握りつぶしてしまっているのが気に食わないのだが、まあいいか。
    */
    static auto is_tty(value_type fd)
    {
      return ::isatty(fd) ? true : false;
    }

    /**
    * 上の自身を対象としたやつ。
    */
    bool is_tty()
    {
      return is_tty(*this);
    }
  };

  /// クラス `file_descriptor` を `fd` と記述したい時のためのエイリアス。
  using fd = file_descriptor;
} // namespace meevax::posix

#endif // INCLUDED_MEEVAX_POSIX_FILE_DESCRIPTOR_HPP

