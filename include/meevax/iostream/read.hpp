#ifndef INCLUDED_MEEVAX_IOSTREAM_READ_HPP
#define INCLUDED_MEEVAX_IOSTREAM_READ_HPP


/**
* @file read.hpp
*
* 種々のフォーマットからのデータ読み取り処理を汎用的に行うための関数オブジェクト型 `read` とその補助クラス定義。
*/


#include <exception>
#include <iostream>
#include <regex>
#include <utility>

#include <boost/cstdlib.hpp>

#include <meevax/type_traits/type_traits.hpp>


/**
* 入出力に関係するクラス・関数を含む名前空間。標準ライブラリ `iostream` に対して最低限の互換性を保証したい。
*/
namespace meevax::iostream
{
  /**
  * あらゆるデータ型、状況に対し汎用的なデータ読み取りを提供する事を目的とした関数オブジェクト型。
  *
  * 技術的には「読み取り」の意味論の下、標準入力ストリーム及びファイルストリームライブラリを
  * ファイルシステムライブラリ等を駆使して統合することが目標。<br>
  */
  template <typename CharType
  #ifndef DOXYGEN_TEMPLATE_SFINAE_CONCEALER
  , typename = typename std::enable_if<meevax::type_traits::is_char_type<CharType>::value>::type
  #endif
  >
  class read
  {
  public:
    /// TODO DOCUMENTATION
    using char_type = CharType;

    /**
    * 読み取りの意味論表現。端的に言えば読み取りの単位。
    *
    * TODO 意味論階層の移動を表現するためインクリメント、デクリメント演算子の適用可能化。
    */
    enum class hierarchical_semantics
    {
      getchar,  ///< 文字読み取り。文字型ひとつ分を読み取る。
      sequence, ///< シーケンス読み取り。賢い文字読み取り。
      getline,  ///< 行読み取り。デリミタ（デフォルトでは改行文字）までを読み取る。
      size
    };

  private:
    /// 入力ストリームへの参照。ここの設計はまだ悩んでる。やっつけ。
    std::basic_istream<char_type>& istream_;

  public:
    /**
    * 手抜きコンストラクタ。一先ず前に進むために最低限だけ実装。
    */
    explicit read(std::basic_istream<char_type>& istream)
      : istream_ {istream}
    {}

    /**
    * 関数呼び出し演算子オーバーロード。
    * 所望の意味をテンプレートパラメータに与えて呼び出すことで、振る舞いが変わる。
    *
    * ## Design Background
    *   本当はこの `read::operator()` で直にテンプレート特殊化を用いたタグディスパッチをやりたかったのだが、
    *   メンバ関数に対しての完全特殊化が文法上許されていなかったので、
    *   関数としての役割をインナー関数オブジェクトクラスで代用した。
    *   インナークラス部分特殊化までしか許されていない都合上、
    *   パターンマッチとしての特殊化は Concept-Controlled Polymorphism で代用した。
    *
    * ## Known Problem
    *   タグディスパッチ用の型をテンプレートパラメータに渡す都合上、
    *   `meevax::iostream::read<char>` 型のオブジェクト `hoge` に対する関数呼び出し演算子のコールが
    *   単純に `hoge<tag_type>()` と書けず、`hoge.operator()<tag_type>()` としなければならない。
    *   ひどい場合は `hoge.template operator()<tag_type>()` とテンプレートの明示すら必要になってしまう。
    *   これは結構問題なのだが、`meevax::iostream::read` はライブラリとして使いやすいことよりも、
    *   読み取り意味論の抽象化を関数オブジェクト型で規格化することが存在意義なのでこれでよい、としている。
    *
    * @tparam HierarchicalSemantics
    *   意味論を示す何かしらのフラグ。型名ではなく値であることに注意。
    *   現状では型が `auto...` となっているが、これは将来の拡張に備えた手抜きであって、
    *   基本的には `meevax::iostream::read::hierarchical_semantics` 以外が与えられることはないはず。
    *
    * @return decltype(auto)
    */
    template <auto... HierarchicalSemantics, typename... Ts>
    decltype(auto) operator()(Ts&&... args)
    {
      return read_<HierarchicalSemantics...> {}(*this, std::forward<Ts>(args)...);
    }

  protected:
    /**
    * Concept-Controlled Polymorphism の起点となるプライベートインナークラスの宣言。
    * このクラスは特殊化が定義されていないパラメータについて実体化されてはならない。
    */
    template <hierarchical_semantics HierarchicalSemantics, typename = void>
    class read_;

    /**
    * 文字読み取り。`std::getchar` とほぼ同様の動作。
    */
    template <hierarchical_semantics HierarchicalSemantics>
    class read_<HierarchicalSemantics, typename std::enable_if<HierarchicalSemantics == hierarchical_semantics::getchar>::type>
    {
    public:
      /**
      * 文字読み取り。
      * `std::getchar` とほぼ同様の動作だが、返り値が `int` ではなく `char_type` である点に注意。
      */
      template <typename ReadType>
      auto operator()(ReadType&& read)
      {
        using read_type = typename std::remove_reference<ReadType>::type;
        return static_cast<typename read_type::char_type>(read.istream_.get());
      }
    };

    /**
    * シーケンス読み取り。
    *
    * 意味としては一文字の範疇だが、データとしてはバイトシーケンスとなっているもの。
    * ASCII 文字入力においてはエスケープシーケンスが該当。
    * 将来的にはユニコードに対応させたい。
    * 入力ストリームからシーケンスを切り分けるという意味で、`std::getline` の別種に相当するか。
    *
    * 返り値型は現在はただの `std::basic_string<char_type>` だが、
    * これを継承して解析用の情報を一緒に持った `class escape_sequence`
    * や `class unicode` を返すようにしたほうが便利だろうか。
    */
    template <hierarchical_semantics HierarchicalSemantics>
    class read_<HierarchicalSemantics, typename std::enable_if<HierarchicalSemantics == hierarchical_semantics::sequence>::type>
    {
    public:
      /**
      * 汚い。要修正。というか処理のいくらかはシーケンス文字ライブラリに移管すべき。
      */
      template <typename ReadType>
      auto operator()(ReadType&& read)
      {
        using read_type = typename std::remove_reference<ReadType>::type;
        using char_type = typename read_type::char_type;

        static const std::basic_regex<char_type> escape_sequence {"\e(\\[[^a-zA-Z]*[a-zA-Z])"};
        std::match_results<typename std::basic_string<char_type>::const_iterator> results {};

        if (const auto head {read.template operator()<read_type::hierarchical_semantics::getchar>()}; is_control_code(head))
        {
          switch (head)
          {
          case 0x1B:
            {
              std::basic_string<char_type> buffer {head};
              while (!std::regex_match(buffer, results, escape_sequence))
              {
                buffer += read.template operator()<read_type::hierarchical_semantics::getchar>();
              }
              return std::move(buffer.replace(0, 1, "^["));
            }

          case 0x7F:
            return std::basic_string<char_type> {"^?"};

          default:
            return std::basic_string<char_type> {"^"} + static_cast<char_type>('@' + head);
          }
        }
        else
        {
          return std::basic_string<char_type> {head};
        }
      }

    private:
      template <typename T>
      bool is_control_code(T code)
      {
        return (0x00 <= code && code <= 0x1F) || code == 0x7F;
      }
    };
  };
} // namespace meevax::iostream


#endif // INCLUDED_MEEVAX_IOSTREAM_READ_HPP

