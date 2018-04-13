#ifndef INCLUDED_MEEVAX_SYNTAX_LEXER_HPP
#define INCLUDED_MEEVAX_SYNTAX_LEXER_HPP


/**
* @file lexer.hpp
*
* meevax 文法解析のためのレキサなどを提供するヘッダ。
* 基本的に機能分離がうまくいってない。
*/


#include <unordered_map>
#include <vector>

#include <meevax/iostream/iostream.hpp>
#include <meevax/regex/regex.hpp>
#include <meevax/type_traits/type_traits.hpp>


namespace meevax::syntax
{
  template <typename CharType
  #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  , typename = typename std::enable_if<
                          meevax::type_traits::is_char_type<CharType>::value
                        >::type
  #endif // DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  >
  class lexer
  {
  public:
    using char_type = CharType;

  private:
    static inline const std::basic_regex<char_type> double_quoted_string {"\"[^\"]*\""};
    static inline const std::basic_regex<char_type> comma_separated_list {"([^\\s,.]+,\\s+)+([^\\s,.]+)?"};

  public:
    std::unordered_map<char_type, std::vector<std::string>> extracted;

  public:
    template <typename... Ts>
    decltype(auto) operator()(Ts&&... args)
    {
      return tokenize(std::forward<Ts>(args)...);
    }

    auto tokenize(std::basic_string<char_type> code)
    {
      extracted['"'] = meevax::regex::extract(code, double_quoted_string, std::basic_string<char_type> {"\""});
      extracted[','] = meevax::regex::extract(code, comma_separated_list, std::basic_string<char_type> {","});

      return std::move(code);
    }
  };
} // namespace meevax::syntax


#endif // INCLUDED_MEEVAX_SYNTAX_LEXER_HPP

