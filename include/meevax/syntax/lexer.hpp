#ifndef INCLUDED_MEEVAX_SYNTAX_LEXER_HPP
#define INCLUDED_MEEVAX_SYNTAX_LEXER_HPP


/**
* @file lexer.hpp
*
* meevax 文法解析のためのレキサなどを提供するヘッダ。
* 基本的に機能分離がうまくいってない。
*/


#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include <meevax/regex/extract.hpp>
#include <meevax/type_traits/is_char_type.hpp>


#ifndef __cpp_nested_namespace_definitions
namespace meevax { namespace syntax
#else
namespace meevax::syntax
#endif
{
  template <typename CharType
  #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  , typename = typename std::enable_if<
                          meevax::type_traits::is_char_type<CharType>::value
                        >::type
  #endif // DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  >
  class [[deprecated]] lexer_
  {
  public:
    using char_type = CharType;

  private:
    static inline const std::basic_regex<char_type> double_quoted_string {"\"[^\"]*\""};
    static inline const std::basic_regex<char_type> comma_separated_list {"([^\\s,.]+,\\s*)+([^\\s,.]*)?"};
    static inline const std::basic_regex<char_type> sentence             {"([^\\.]+)\\."};

    std::unordered_map<char_type, std::vector<std::string>> extracted;

  public:
    template <typename... Ts>
    [[deprecated]] decltype(auto) operator()(Ts&&... args)
    {
      return translate(std::forward<Ts>(args)...);
    }

    auto translate(std::basic_string<char_type> code)
    {
      extracted['"'] = meevax::regex::extract<std::vector>(
                         code, double_quoted_string, std::basic_string<char_type> {'"'}
                       );
      extracted[','] = meevax::regex::extract<std::vector>(
                         code, comma_separated_list, std::basic_string<char_type> {','}
                       );

      #ifndef NDEBUG
      for (const auto& each_map : extracted)
      {
        std::size_t index {0};
        for (const auto& parts : each_map.second)
        {
          std::cout << "  (" << each_map.first << ")[" << index++ << "] " << parts << "$\r\n";
        }
      }
      #endif

      std::basic_string<char_type> buffer {to_s_expression(code, ' ')};

      // 構造を整理するのは後回し
      if (!extracted.at(',').empty())
      {
        typename decltype(extracted)::mapped_type::size_type index {0};
        for (typename std::basic_string<char_type>::size_type position {0};
             std::basic_string<char_type>::npos != (position = buffer.find(',', position));
             ++index)
        {
          buffer.replace(position, 1,
            to_s_expression(
              replace_comma_to_space(extracted.at(',')[index]), ' '
            )
          );
          position += extracted.at(',')[index].length();
        }
      }

      // 文字列リテラルの中には他の構文に引っかかる文字が入っている可能性がありすぎるので、
      // コイツを詰め戻すのは最後の工程に回すべき
      if (!extracted['"'].empty())
      {
        typename decltype(extracted)::mapped_type::size_type index {0};
        for (typename std::basic_string<char_type>::size_type position {0};
             std::basic_string<char_type>::npos != (position = buffer.find('"', position)) && index < extracted['"'].size();
             ++index)
        {
          buffer.replace(position, 1, {extracted['"'][index]});
          position += extracted['"'][index].length();
        }
      }

      return std::move(buffer);
    }

  private:
    /**
    * CSL をスペース区切り文字列に変換する。余分なスペースは削除される。
    */
    auto replace_comma_to_space(std::basic_string<char_type> source)
    {
      source.erase(std::remove(std::begin(source), std::end(source), ' '), std::end(source));
      std::replace(std::begin(source), std::end(source), ',', ' ');
      return std::move(source);
    }

    /**
    * スペース区切り文字列を S 式へ変換する
    */
    auto to_s_expression(std::basic_istringstream<char_type> source, char_type delimiter)
      -> std::basic_string<char_type>
    {
      std::basic_ostringstream<char_type> result {};
      for (std::basic_string<char_type> buffer {}; std::getline(source, buffer, delimiter); )
      {
        result << "(" << buffer << ")";
      }

      return {result.str()};
    }

    decltype(auto) to_s_expression(std::basic_string<char_type> source, char_type delimiter)
    {
      return to_s_expression(std::basic_istringstream<char_type> {source}, delimiter);
    }
  };

  /**
  * 構文解析クラス。
  * 仕事は meevax コードを S 式へ変換し、その際に収集した情報を保持しておくこと。
  *
  * 設計方針として、徹底して冗長化を施しておきメンテしやすくしておくことを重視する。
  * 加えて、解析の各段階でデバッグを行うための文字列出力を丁寧にサポートしておく。
  * つまり、実行速度は現時点で一切気にしない。
  *
  * 構文解析を行うが、直接 AST を構築することはしない。
  * それは、別途 S 式から AST の構築を行うクラス（lisp に関連付ける方針）に任せる。
  */
  template <typename CharType
  #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  , typename = typename std::enable_if<
                          meevax::type_traits::is_char_type<CharType>::value
                        >::type
  #endif // DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  >
  class lexer
  {
  };
} // namespace meevax::syntax
#ifndef __cpp_nested_namespace_definitions
}
#endif


#endif // INCLUDED_MEEVAX_SYNTAX_LEXER_HPP

