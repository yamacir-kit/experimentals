#ifndef INCLUDED_MEEVAX_TRIAL_ANSI_ESCAPE_SEQENCE_HPP_
#define INCLUDED_MEEVAX_TRIAL_ANSI_ESCAPE_SEQENCE_HPP_


#include <ostream>
#include <string>

#include <cstdint>

#include <meevax/trial/constexpr_string.hpp>


namespace meevax {


class ansi_escape_seqence
  : public meevax::constexpr_string<char>
{
public:
  static constexpr char escape {0x1b};

  class text_attribute
    : public meevax::constexpr_string<char>
  {
  public:
    static constexpr meevax::constexpr_string<char> reset      {"0"};
    static constexpr meevax::constexpr_string<char> bold       {"1"};
    static constexpr meevax::constexpr_string<char> underscore {"4"};
    static constexpr meevax::constexpr_string<char> blink      {"5"};
    static constexpr meevax::constexpr_string<char> reverse    {"7"};
    static constexpr meevax::constexpr_string<char> consealed  {"8"};
  };

  class foreground_color
    : public meevax::constexpr_string<char>
  {
  public:
    static constexpr meevax::constexpr_string<char> black   {"30"};
    static constexpr meevax::constexpr_string<char> red     {"31"};
    static constexpr meevax::constexpr_string<char> green   {"32"};
    static constexpr meevax::constexpr_string<char> yellow  {"33"};
    static constexpr meevax::constexpr_string<char> blue    {"34"};
    static constexpr meevax::constexpr_string<char> magenta {"35"};
    static constexpr meevax::constexpr_string<char> cyan    {"36"};
    static constexpr meevax::constexpr_string<char> white   {"37"};
  };

  class background_color
    : public meevax::ansi_escape_seqence::foreground_color
  {};
};


} // namespace meevax


#endif
