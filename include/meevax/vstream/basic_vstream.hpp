#ifndef INCLUDED_MEEVAX_VSTREAM_BASIC_VSTREAM_HPP_
#define INCLUDED_MEEVAX_VSTREAM_BASIC_VSTREAM_HPP_


#include <memory>
#include <string>
#include <unordered_map>

#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>


namespace meevax {


template <typename C>
class basic_surface;


template <typename C>
class basic_surface
  : public std::unique_ptr<cairo_t, decltype(&cairo_destroy)>
{
  std::unordered_map<std::basic_string<C>, meevax::basic_surface<C>> sub_surfaces_;

public:
  template <typename... Ts>
  explicit basic_surface(Ts&&... args)
    : std::unique_ptr<cairo_t, decltype(&cairo_destroy)> {std::forward<Ts>(args)...}
  {}

protected:
  // create(const std::basic_string<C>& sub_surface);
};


template <typename C>
class basic_vstream
{
  std::unique_ptr<Display, decltype(&XCloseDisplay)> display_;

  std::unordered_map<
    std::basic_string<C>,
    std::unique_ptr<cairo_t, decltype(&cairo_destroy)>
  > surfaces_;

public:
  basic_vstream(const std::basic_string<C>& name = {""});

  auto& operator[](const std::basic_string<C>& surface)
  {
    return surfaces_.emplace(surface, create(surface)).first->second;
  }

  auto next_event()
  {
    static XEvent event {};
    XNextEvent(display_.get(), &event);
    return event;
  }

private:
  auto create(const std::basic_string<C>& target_surface,
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

#ifndef NDEBUG
  XSynchronize(display_.get(), true);
#endif
}


template <typename C>
auto meevax::basic_vstream<C>::create(const std::basic_string<C>& target_surface,
                                      const std::basic_string<C>& parent_surface)
{
  constexpr std::size_t width  {16 * 40}; // TODO
  constexpr std::size_t height { 9 * 40}; // TODO
  constexpr std::size_t border_line_width {0};
  auto     border_pixel_color {XBlackPixel(display_.get(), XDefaultScreen(display_.get()))};
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

  XSelectInput(display_.get(), simple_window, ExposureMask | KeyPressMask);

  auto cairo_surface {
    cairo_xlib_surface_create(
      display_.get(),
      simple_window,
      XDefaultVisual(display_.get(), XDefaultScreen(display_.get())),
      width,
      height
    )
  };

  return typename decltype(surfaces_)::mapped_type {cairo_create(cairo_surface), cairo_destroy};
}


#endif
