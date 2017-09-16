#ifndef INCLUDED_MEEVAX_STRING_REPLACE_UNPRINTABLE_HPP
#define INCLUDED_MEEVAX_STRING_REPLACE_UNPRINTABLE_HPP


#include <array>
#include <string>
#include <utility>
#include <vector>


namespace meevax::string {


// TODO
// meevax::xcb::ascii_keyboard とも強調するように、
// ライブラリ内で統一的に使える static で constexpr な
// アスキーコード、エスケープシーケンス、キャレットのデータを作るべき


// XXX UGLY CODE
template <typename Char>
auto replace_unprintable(const std::basic_string<Char>& source)
{
  auto buffer {source};

  static const std::vector<std::pair<std::basic_string<Char>, std::basic_string<Char>>> targets {
    {"\e", "\\e"},
    {"\n", "\\n"},
    {"\t", "\\t"}
  };

  for (const auto& target : targets)
  {
    for (auto position {buffer.find(target.first)}; position != std::basic_string<Char>::npos; )
    {
      buffer.replace(position, target.first.size(), target.second);
      position = buffer.find(target.first, position + target.second.size());
    }
  }

  return buffer;
}


} // namespace meevax


#endif

