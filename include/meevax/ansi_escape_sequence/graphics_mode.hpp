#ifndef INCLUDED_MEEVAX_ANSI_ESCAPE_SEQUENCE_GRAPHICS_MODE_HPP
#define INCLUDED_MEEVAX_ANSI_ESCAPE_SEQUENCE_GRAPHICS_MODE_HPP


#include <meevax/string/static_concat.hpp>


namespace meevax::ansi_escape_sequence::attributes {

static constexpr auto off        {meevax::string::static_concat("\e[0m")};
static constexpr auto bold       {meevax::string::static_concat("\e[1m")};
static constexpr auto unserscore {meevax::string::static_concat("\e[4m")};
static constexpr auto blink      {meevax::string::static_concat("\e[5m")};
static constexpr auto reverse    {meevax::string::static_concat("\e[7m")};
static constexpr auto concealed  {meevax::string::static_concat("\e[8m")};

} // meevax::ansi_escape_sequence::attributes


namespace meevax::ansi_escape_sequence::color::foreground {

static constexpr auto black   {meevax::string::static_concat("\e[30m")};
static constexpr auto red     {meevax::string::static_concat("\e[31m")};
static constexpr auto green   {meevax::string::static_concat("\e[32m")};
static constexpr auto yellow  {meevax::string::static_concat("\e[33m")};
static constexpr auto blue    {meevax::string::static_concat("\e[34m")};
static constexpr auto magenta {meevax::string::static_concat("\e[35m")};
static constexpr auto cyan    {meevax::string::static_concat("\e[36m")};
static constexpr auto white   {meevax::string::static_concat("\e[37m")};

} // meevax::ansi_escape_sequence::color::foreground


namespace meevax::ansi_escape_sequence::color::background {

static constexpr auto black   {meevax::string::static_concat("\e[40m")};
static constexpr auto red     {meevax::string::static_concat("\e[41m")};
static constexpr auto green   {meevax::string::static_concat("\e[42m")};
static constexpr auto yellow  {meevax::string::static_concat("\e[43m")};
static constexpr auto blue    {meevax::string::static_concat("\e[44m")};
static constexpr auto magenta {meevax::string::static_concat("\e[45m")};
static constexpr auto cyan    {meevax::string::static_concat("\e[46m")};
static constexpr auto white   {meevax::string::static_concat("\e[47m")};

} // meevax::ansi_escape_sequence::color::background


#endif

