#ifndef INCLUDED_MEEVAX_VISUAL_VISUAL_MANIPULATOR_HPP_
#define INCLUDED_MEEVAX_VISUAL_VISUAL_MANIPULATOR_HPP_


#include <meevax/visual/visual_context.hpp>

#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>


namespace meevax {


visual_context& map(visual_context& vc) noexcept
{
  XMapWindow(vc, vc);
  return vc;
}


visual_context& unmap(visual_context& vc) noexcept
{
  XUnmapWindow(vc, vc);
  return vc;
}


visual_context& map_raised(visual_context& vc) noexcept
{
  XMapRaised(vc, vc);
  return vc;
}


visual_context& flush(visual_context& vc)
{
  cairo_surface_flush(vc);
  XFlush(vc);
  return vc;
}


} // namespace meevax


#endif
