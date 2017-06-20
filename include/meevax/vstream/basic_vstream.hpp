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
  std::unordered_map<
    std::basic_string<C>,
    std::unique_ptr<meevax::basic_surface<C>>
  > sub_surfaces_;

  static constexpr std::size_t default_window_width  {1280};
  static constexpr std::size_t default_window_height { 720};
  static constexpr std::size_t default_border_width  {   1};

public:
  template <typename... Ts>
  basic_surface(Ts&&... args)
    : std::unique_ptr<cairo_t, decltype(&cairo_destroy)> {std::forward<Ts>(args)...}
  {}

  auto& operator[](const std::basic_string<C>& surface_name)
  {
    sub_surfaces_.emplace(
      surface_name,
      create(
        cairo_xlib_surface_get_display(cairo_get_target(*this)),
        cairo_xlib_surface_get_drawable(cairo_get_target(*this))
      )
    );

    return *sub_surfaces_.at(surface_name);
  }

  operator cairo_t*() const noexcept
  {
    return (*this).get();
  }

protected:
  static auto create(Display* display, const Window& window)
    -> meevax::basic_surface<C>
  {
    auto simple_window {XCreateSimpleWindow(display, window, 0, 0, default_window_width, default_window_height, default_border_width, XBlackPixel(display, XDefaultScreen(display)), XWhitePixel(display, XDefaultScreen(display)))};
    auto cairo_surface {cairo_xlib_surface_create(display, simple_window, XDefaultVisual(display, XDefaultScreen(display)), default_window_width, default_window_height)};

    XSelectInput(display, simple_window, ExposureMask | KeyPressMask);

    return {cairo_create(cairo_surface), cairo_destroy};
  }
};


template <typename C>
class basic_vstream
  : public std::unique_ptr<Display, decltype(&XCloseDisplay)>,
    public meevax::basic_surface<C>
{
public:
  basic_vstream(const std::basic_string<C>& display_name = {""})
    : std::unique_ptr<Display, decltype(&XCloseDisplay)> {XOpenDisplay(display_name.c_str()), XCloseDisplay},
      meevax::basic_surface<C> {meevax::basic_surface<C>::create(*this, XDefaultRootWindow(*this))}
  {
    if (*this == nullptr)
    {
      std::cerr << "[error] XOpenDisplay(3) - failed to open display " << display_name << std::endl;
      std::exit(EXIT_FAILURE);
    }

#ifndef NDEBUG
    XSynchronize(*this, true);
#endif
  }

  operator Display*() const noexcept
  {
    return std::unique_ptr<Display, decltype(&XCloseDisplay)>::get();
  }

  auto next_event()
  {
    static XEvent event {};
    XNextEvent(*this, &event);
    return event;
  }
};


} // namespace meevax


#endif
