#ifndef INCLUDED_MEEVAX_TRIAL_ANSI_ESCAPE_SEQENCE_HPP_
#define INCLUDED_MEEVAX_TRIAL_ANSI_ESCAPE_SEQENCE_HPP_


#include <ostream>
#include <string>
#include <array>

#include <cstdint>


namespace meevax {


// class ansi_escape_seqence
// {
//   static constexpr int8_t escape {0x1b}; // 0d27, 033
//   constexpr char*
//
// public:
//   constexpr explicit ansi_escape_seqence()
// };

class text_attribute
{
public:
  static constexpr int_fast8_t reset      {0};
  static constexpr int_fast8_t bold       {1};
  static constexpr int_fast8_t underscore {4};
  static constexpr int_fast8_t blink      {5};
  static constexpr int_fast8_t reverse    {7};
  static constexpr int_fast8_t consealed  {8};
};


class foreground_color
{
public:
  static constexpr int_fast8_t black   {30};
  static constexpr int_fast8_t red     {31};
  static constexpr int_fast8_t green   {32};
  static constexpr int_fast8_t yellow  {33};
  static constexpr int_fast8_t blue    {34};
  static constexpr int_fast8_t magenta {35};
  static constexpr int_fast8_t cyan    {36};
  static constexpr int_fast8_t white   {37};
};


class background_color
{
public:
  static constexpr int_fast8_t black   {40};
  static constexpr int_fast8_t red     {41};
  static constexpr int_fast8_t green   {42};
  static constexpr int_fast8_t yellow  {43};
  static constexpr int_fast8_t blue    {44};
  static constexpr int_fast8_t magenta {45};
  static constexpr int_fast8_t cyan    {46};
  static constexpr int_fast8_t white   {47};
};


} // namespace meevax


#endif
