#ifndef INCLUDED_MEEVAX_VISUAL_VISUAL_STREAM_HPP_
#define INCLUDED_MEEVAX_VISUAL_VISUAL_STREAM_HPP_


#include <string>
#include <unordered_map>

#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>

#include <meevax/visual/visual_context.hpp>
#include <meevax/visual/xlib_display.hpp>


namespace meevax {


class visual_stream
{
  meevax::xlib_display display_;

  meevax::visual_context master_;
  std::unordered_map<std::string, meevax::visual_context> contexts_;

private:
  auto default_root_window() const noexcept
  {
    return XDefaultRootWindow(display_);
  }

  auto default_screen() const noexcept
  {
    return XDefaultScreen(display_);
  }

  auto default_visual() const noexcept
  {
    return XDefaultVisual(display_, default_screen());
  }

  auto white_pixel() const noexcept
  {
    return XWhitePixel(display_, default_screen());
  }

  auto black_pixel() const noexcept
  {
    return XBlackPixel(display_, default_screen());
  }

private:
  auto create_surface(Window parents, std::size_t width, std::size_t height) const
  {
    auto window {XCreateSimpleWindow(display_, parents, 0, 0, width, height, 0, black_pixel(), white_pixel())};
    return cairo_xlib_surface_create(display_, window, default_visual(), width, height);
  }

public:
  visual_stream(const std::string& name, std::size_t width, std::size_t height)
    : display_ {name},
      master_ {create_surface(default_root_window(), width, height)},
      contexts_ {}
  {
    XSynchronize(display_, true); // for debug
  }

  visual_context& operator[](const std::string& s)
  {
    if (contexts_.find(s) == contexts_.end())
    {
      contexts_.emplace(s, create_surface(master_, 160, 90));
    }

    return contexts_.at(s);
  }
};


} // namespace meevax


#endif
