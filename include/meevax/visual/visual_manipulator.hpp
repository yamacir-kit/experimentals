#ifndef INCLUDED_MEEVAX_VISUAL_VISUAL_MANIPULATOR_HPP_
#define INCLUDED_MEEVAX_VISUAL_VISUAL_MANIPULATOR_HPP_


#include <string>

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


class color
{
  double red_, green_, blue_, alpha_;

public:
  color(double r, double g, double b, double a = 1.0)
    : red_ {r}, green_ {g}, blue_ {b}, alpha_ {a}
  {}

  visual_context& operator()(visual_context& vc)
  {
    cairo_set_source_rgba(vc, red_, green_, blue_, alpha_);
    return vc;
  }
};


visual_context& paint(visual_context& vc)
{
  cairo_paint(vc);
  return vc;
}


class font_face
{
  const std::string name_;
  cairo_font_slant_t slant_;
  cairo_font_weight_t weight_;

public:
  font_face(const std::string& name,
            cairo_font_slant_t slant = CAIRO_FONT_SLANT_NORMAL,
            cairo_font_weight_t weight = CAIRO_FONT_WEIGHT_NORMAL)
    : name_ {name}, slant_ {slant}, weight_ {weight}
  {}

  visual_context& operator()(visual_context& vc)
  {
    cairo_select_font_face(vc, name_.c_str(), slant_, weight_);
    return vc;
  }
};


} // namespace meevax


#endif
