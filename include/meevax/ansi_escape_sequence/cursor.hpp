#ifndef INCLUDED_MEEVAX_ANSI_ESCAPE_SEQUENCE_CURSOR_HPP
#define INCLUDED_MEEVAX_ANSI_ESCAPE_SEQUENCE_CURSOR_HPP


#include <meevax/string/static_concat.hpp>


namespace meevax::ansi_escape_sequence::cursor {


#ifdef _WIN32
static constexpr auto newline {meevax::string::static_concat("\r\n")};
#elif defined macintosh
static constexpr auto newline {meevax::string::static_concat("\r")};
#else
static constexpr auto newline {meevax::string::static_concat("\n")};
#endif


} // namespace meevax::ansi_escape_sequence::cursor


#endif

