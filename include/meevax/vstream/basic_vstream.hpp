#ifndef INCLUDED_MEEVAX_VSTREAM_BASIC_VSTREAM_HPP_
#define INCLUDED_MEEVAX_VSTREAM_BASIC_VSTREAM_HPP_


#include <string>
#include <unordered_map>
#include <memory>

#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>


namespace meevax {

template <typename C>
class basic_vstream
{
  std::unique_ptr<Display, decltype(&XCloseDisplay)> display_;

  std::unordered_map<
    std::string,
    std::unique_ptr<cairo_t, decltype(&cairo_destroy)>
  > surfaces_;

public:
  basic_vstream(const std::basic_string<C>& name = {""});

  // create();
  auto create(const std::basic_string<C>& surface_name,
              const std::basic_string<C>& parent_surface = {""});
};

} // namespace meevax


template <typename C>
meevax::basic_vstream<C>::basic_vstream(const std::basic_string<C>& name)
  : display_ {XOpenDisplay(name.c_str()), XCloseDisplay},
    surfaces_ {}
{
  if (display_ == nullptr)
  {
    std::cerr << "[error] XOpenDisplay(3) - failed to open display " << name << std::endl;
    std::exit(EXIT_FAILURE);
  }
}


template <typename C>
auto meevax::basic_vstream<C>::create(const std::basic_string<C>& surface_name,
                                      const std::basic_string<C>& parent_surface)
{
  std::size_t width {320}; // TODO
  std::size_t height {240}; // TODO
  std::size_t border_line_width {1};
  auto border_pixel_color {XBlackPixel(display_.get(), XDefaultScreen(display_.get()))};
  auto background_pixel_color {XWhitePixel(display_.get(), XDefaultScreen(display_.get()))};

  auto simple_window {
    XCreateSimpleWindow(
      display_.get(),
      parent_surface.empty() ? XDefaultRootWindow(display_.get()) : cairo_xlib_surface_get_drawable(cairo_get_target(surfaces_.at(parent_surface).get())),
      0, // position x form parent window
      0, // position y form parent window
      width,
      height,
      border_line_width,
      border_pixel_color,
      background_pixel_color
    )
  };

  auto cairo_surface {
    cairo_xlib_surface_create(
      display_.get(),
      simple_window,
      XDefaultVisual(display_.get(), XDefaultScreen(display_.get())),
      width,
      height
    )
  };

  surfaces_.emplace(
    surface_name,
    typename decltype(surfaces_)::mapped_type {cairo_create(cairo_surface), cairo_destroy}
  );

  XMapRaised(
    display_.get(),
    cairo_xlib_surface_get_drawable(cairo_get_target(surfaces_.at(surface_name).get()))
  );
}


#endif
