#ifndef INCLUDED_MEEVAX_REGEX_EXTRACT_HPP
#define INCLUDED_MEEVAX_REGEX_EXTRACT_HPP


/**
* @file extract.hpp
*
* テンプレート関数 `extract` を提供するヘッダ。
*/


#include <iterator>
#include <regex>
#include <vector>

#include <meevax/string/string.hpp>
#include <meevax/type_traits/type_traits.hpp>


namespace meevax::regex
{
  /**
  * 指定された文字列から、正規表現に合致する部分文字列を抽出する。
  * 極めて限定的な目的に使うため作ったからあまり気に入ってない。
  *
  * 抽出された部分文字列は標準 STL コンテナに詰められて返され、
  * それがもともとあった場所には第三引数に指定された文字列が挿入される（デフォルトでは何も挿入されない）。
  *
  * @param from
  *   抽出対象となる文字列。この関数によって書き換えられることに注意。
  *
  * @param regex
  *   抽出したい部分文字列の正規表現。
  *
  * @param replaced
  *   部分文字列を抜き出すとき、代わりに挿入される文字列。
  *
  * @return
  *   抽出された文字列を標準コンテナに詰めたもの。
  *   まあ基本的に `std::vector<std::string>` と思っておいて問題ない。
  */
  template <typename CharType, template <typename...> typename StandardContainer = std::vector
  #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  , typename = typename std::enable_if<
                          meevax::type_traits::is_char_type<CharType>::value
                        >::type
  , typename = typename std::enable_if<
                          !std::is_same<
                            StandardContainer<CharType>,
                            std::basic_string<CharType>
                          >::value
                        >::type
  #endif // DOXYGEN_TEMPLATE_SFINAE_CONCLEALER
  >
  auto extract(std::basic_string<CharType>& from, const std::basic_regex<CharType>& regex, const std::basic_string<CharType>& replaced = "")
  {
    StandardContainer<std::basic_string<CharType>> extracted {};
    using regex_iterator = std::regex_iterator<typename std::basic_regex<CharType>::const_iterator>;

    for (regex_iterator iter {std::cbegin(from), std::cend(from), regex}; iter != regex_iterator {}; iter)
    {
      extracted.emplace((*iter)[0]);
    }
    from = std::regex_replace(from, regex, replaced);

    return std::move(extracted);
  }
}; // namespace meevax::regex


#endif // INCLUDED_MEEVAX_REGEX_EXTRACT_HPP

