#ifndef INCLUDED_MEEVAX_POSIX_POSIX_HPP
#define INCLUDED_MEEVAX_POSIX_POSIX_HPP


/**
* @file posix.hpp
*
* 名前空間 `meevax::posix` を定義するヘッダ群の一括インクルードを提供するヘッダ。
* また、POSIX システム標準ヘッダ `unistd.h` をインクルードする。Windows は知らん。
*/


#include <meevax/posix/file_descriptor.hpp>
#include <meevax/posix/termios.hpp>


/**
* POSIX 標準ライブラリの C++ フレンドリなラッパを含む名前空間。
* 名前を unix にするか結構迷ったが、一先ずこちらにした。
*/
namespace meevax::posix {};


#endif // INCLUDED_MEEVAX_POSIX_POSIX_HPP

