#ifndef INCLUDED_MEEVAX_VISUAL_VISUAL_CONTEXT_HPP_
#define INCLUDED_MEEVAX_VISUAL_VISUAL_CONTEXT_HPP_


#include <memory>

#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>


namespace meevax {


class visual_context
{
  std::unique_ptr<cairo_t, decltype(&cairo_destroy)> cairo_;

public:
  explicit visual_context(cairo_surface_t* surface)
    : cairo_ {cairo_create(surface), cairo_destroy}
  {}

  visual_context& operator<<(visual_context& (*manipulator)(visual_context&))
  {
    return (*manipulator)(*this);
  }
};


} // namespace meevax


#endif
