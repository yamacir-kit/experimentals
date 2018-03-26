#ifndef INCLUDED_MEEVAX_POSIX_TERMIOS_HPP
#define INCLUDED_MEEVAX_POSIX_TERMIOS_HPP


/**
* @file termios.hpp
*
* POSIX 標準ライブラリ `termios.h` を使うたびに参考書をめくるのが面倒になったから作った。
* 結局のところ使いたい機能は限定的だろうと言う甘い目算の元開発。
*/


#include <cerrno>
#include <stdexcept>
#include <system_error>
#include <utility>

#include <termios.h>
#include <unistd.h>

#include <meevax/type_traits/is_scoped_bitmasks.hpp>


/**
* POSIX 標準ライブラリの C++ フレンドリなラッパを含む名前空間。
* 名前を unix にするか結構迷ったが、一先ずこちらにした。
*/
namespace meevax::posix
{
  class fd
  {
  public:
    using value_type = int;

  public:
    value_type value;

    static constexpr value_type stdin  { STDIN_FILENO};
    static constexpr value_type stdout {STDOUT_FILENO};
    static constexpr value_type stderr {STDERR_FILENO};

  public:
    fd(value_type value)
      : value {value}
    {}

    operator value_type()
    {
      return value;
    }
  };


  /**
  * POSIX 標準ライブラリの構造体 `termios` の拡張。
  *
  * 構造体 `{annonymous}::termios` から派生しているため、`termios.h` 内の各関数の引数として指定可能。<br>
  * 現状は間に合わせの実装なのでカノニカルモードの設定くらいしかサポートしていない。
  *
  * メモ：`termios` の拡張であることを第一義とするため、使いやすくする以上のことはしない。
  */
  class termios
    : public ::termios
  {
  public:
    /**
    * 端末ドライバの入力モードを示すフラグ値。型安全にビット幅演算可能。
    *
    * 主にメンバ関数 `set_attrib` への引数として用いる。<br>
    * 詳細情報は The Linux Programming Interface pp.1356-1357 参照。
    */
    enum class input_mode
      : unsigned int
    {
         canonical = 1 << 0, ///< 端末からの入力を行単位に処理し、また行編集を可能とするモード。
      noncanonical = 1 << 1, ///< 端末からの入力を行単位にまとめないモード。
    };

    /**
    * 端末属性の変更を行う際の、変更がいつから効果を発揮するかを指定するためのフラグ値。
    *
    * 元々、`termios.h` 内で `optional_actions` と呼ばれていたものだが、分かりにくいので変更した。<br>
    * こちらではなく、`when` という名称も良いかなとは思ったが、文章になる方を選んだ。<br>
    * 詳細情報は The Linux Programming Interface pp.1358-1359 参照。<br>
    */
    enum class effects
      : int
    {
         from_now = TCSANOW,   ///< 変更はその場ですぐに効果を発揮する。
      after_drain = TCSADRAIN, ///< 変更は現在キューイングされている出力をすべて端末に転送後に効果を発揮する。
      after_flush = TCSAFLUSH, ///< 現在キューイングされている出力を破棄し、変更はその後すぐに効果を発揮する。
    };

  private:
    const int fd_;
    const struct ::termios initial_attrib_;

  public:
    explicit termios(decltype(fd_) fd)
      : fd_ {::isatty(fd) ? fd : throw std::system_error {errno, std::system_category()}},
        initial_attrib_ {get_attrib(fd_)}
    {}

    ~termios()
    {
      ::tcsetattr(fd_, static_cast<std::underlying_type<effects>::type>(effects::from_now), &initial_attrib_);
    }

    /**
    * 元々 `tcsetattr` だったもの。名前を変えたことに特に深い意味はないがパッと見が呪文だったので変えた。
    *
    * 元の意味は terminal i/o control set attributes だろうか。
    */
    void set_attrib(effects when = effects::from_now) const
    {
      if (const auto status {::tcsetattr(fd_, static_cast<std::underlying_type<effects>::type>(when), this)}; status < 0)
      {
        throw std::system_error {errno, std::system_category()};
      }
    }

    static struct ::termios get_attrib(int fd)
    {
      struct ::termios buffer {};
      if (const auto status {::tcgetattr(fd, &buffer)}; status < 0)
      {
        throw std::system_error {errno, std::system_category()};
      }
      else
      {
        return std::move(buffer);
      }
    }

    void change_to(input_mode mode)
    {
      switch (mode)
      {
      case meevax::posix::termios::input_mode::canonical:
        (*this).c_lflag &= ~(ICANON | ECHO);
        (*this).c_cc[VMIN]  = 1;
        (*this).c_cc[VTIME] = 0;

        set_attrib(effects::from_now);
        break;

      case meevax::posix::termios::input_mode::noncanonical:
        throw std::runtime_error {
          "[error] meevax::posix::termios::change_to() - unimplemented mode: noncanonical"
        };
      }
    }
  };
} // namespace meevax::posix


namespace meevax::type_traits
{
  /**
  * クラステンプレート `is_scoped_bitmasks` の特殊化定義。`std::true_type` から派生する。
  *
  * こいつにビット幅演算が必要かどうかは実のところよく分からない。多分要らない。
  */
  template <>
  class is_scoped_bitmasks<enum meevax::posix::termios::input_mode>
    : public std::true_type
  {};
} // namespace meevax::type_traits


#endif // INCLUDED_MEEVAX_POSIX_TERMIOS_HPP

