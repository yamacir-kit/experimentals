#ifndef INCLUDED_MEEVAX_VISUAL_VISUAL_STREAM_HPP_
#define INCLUDED_MEEVAX_VISUAL_VISUAL_STREAM_HPP_


#include <string>

#include <meevax/visual/visual_context.hpp>
#include <meevax/visual/xlib_display.hpp>


namespace meevax {


class visual_stream
{
  meevax::xlib_display display_;

public:
  visual_stream(const std::string& name = {""})
    : display_ {name}
  {}
};


} // namespace meevax


#endif
