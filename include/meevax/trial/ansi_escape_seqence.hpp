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
  static constexpr meevax::constexpr_string<char> delimiter {";"};

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
    : public meevax::constexpr_string<char>
  {
  public:
    static constexpr meevax::constexpr_string<char> black   {"40"};
    static constexpr meevax::constexpr_string<char> red     {"41"};
    static constexpr meevax::constexpr_string<char> green   {"42"};
    static constexpr meevax::constexpr_string<char> yellow  {"43"};
    static constexpr meevax::constexpr_string<char> blue    {"44"};
    static constexpr meevax::constexpr_string<char> magenta {"45"};
    static constexpr meevax::constexpr_string<char> cyan    {"46"};
    static constexpr meevax::constexpr_string<char> white   {"47"};
  };

  class graphics_mode
    : public text_attribute,
      public foreground_color,
      public background_color
  {};

  template <typename T>
  constexpr ansi_escape_seqence(T&& code)
    : meevax::constexpr_string<char>("\e[" + code)
  {}

  template <typename T, typename... P>
  constexpr ansi_escape_seqence(T&& code, P&&... args)
    : meevax::constexpr_string<char>(*this ? (*this).data() + delimiter.data() + code.data() : code.data())
  {
    ansi_escape_seqence(args...);
  }
};


} // namespace meevax


#endif
