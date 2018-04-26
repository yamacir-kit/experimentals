#ifndef INCLUDED_MEEVAX_REGEX_PREDEFINED_HPP
#define INCLUDED_MEEVAX_REGEX_PREDEFINED_HPP


/**
* @file predefined.hpp
*
* 事前に定義しておいたほうが便利な正規表現セットを提供するヘッダ。
*/


#include <regex>
#include <type_traits>

#include <meevax/type_traits/is_char_type.hpp>


namespace meevax::regex
{
  /**
  * ANSI エスケープシーケンス。
  * どうやらエスケープシーケンスであるらしい文を捉えることが目的。
  *
  * 存在しないはずのエスケープシーケンスを書いても基本形式が合っていればマッチしてしまう。
  * より賢いエスケープシーケンスの解析は TODO。
  */
  template <typename CharType
  #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  , typename = typename std::enable_if<
                          meevax::type_traits::is_char_type<CharType>::value
                        >::type
  #endif // DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  >
  static const std::basic_regex<CharType> ansi_escape_sequence {"\e(\\[[^:alpha:]*[:alpha:])"};
}; // meevax::regex


#endif // INCLUDED_MEEVAX_REGEX_PREDEFINED_HPP

