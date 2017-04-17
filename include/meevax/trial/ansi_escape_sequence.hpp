#ifndef INCLUDED_MEEVAX_TRIAL_ANSI_ESCAPE_SEQENCE_HPP_
#define INCLUDED_MEEVAX_TRIAL_ANSI_ESCAPE_SEQENCE_HPP_


#include <ostream>
#include <string>

#include <cstdint>

#include "meevax/trial/constexpr_string.hpp"
#include "meevax/trial/static_concatenate.hpp"


namespace posix {


template <typename C>
class ansi_escape_seqence
{
  using char_type = typename std::basic_string<C>::value_type;

  static constexpr trial::static_concatenate<char_type> scat {};

public:
  static constexpr auto delimiter {scat(";")};

  class text_attribute
  {
  public:
    static constexpr auto reset      {scat("0")};
    static constexpr auto bold       {scat("1")};
    static constexpr auto underscore {scat("4")};
    static constexpr auto blink      {scat("5")};
    static constexpr auto reverse    {scat("7")};
    static constexpr auto consealed  {scat("8")};
  };

  class foreground_color
  {
  public:
    static constexpr auto black   {scat("30")};
    static constexpr auto red     {scat("31")};
    static constexpr auto green   {scat("32")};
    static constexpr auto yellow  {scat("33")};
    static constexpr auto blue    {scat("34")};
    static constexpr auto magenta {scat("35")};
    static constexpr auto cyan    {scat("36")};
    static constexpr auto white   {scat("37")};
  };

  class background_color
    : public meevax::constexpr_string<char>
  {
  public:
    static constexpr auto black   {scat("40")};
    static constexpr auto red     {scat("41")};
    static constexpr auto green   {scat("42")};
    static constexpr auto yellow  {scat("43")};
    static constexpr auto blue    {scat("44")};
    static constexpr auto magenta {scat("45")};
    static constexpr auto cyan    {scat("46")};
    static constexpr auto white   {scat("47")};
  };

  // template <typename T>
  // constexpr ansi_escape_seqence(T&& code)
  //   : meevax::constexpr_string<char>("\e[" + code)
  // {}
  //
  // template <typename T, typename... P>
  // constexpr ansi_escape_seqence(T&& code, P&&... args)
  //   : meevax::constexpr_string<char>(*this ? (*this).data() + delimiter.data() + code.data() : code.data())
  // {
  //   ansi_escape_seqence(args...);
  // }
};


} // namespace posix


#endif
