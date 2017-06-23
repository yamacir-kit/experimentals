#ifndef INCLUDED_MEEVAX_VSTREAM_BASIC_VSTREAM_HPP_
#define INCLUDED_MEEVAX_VSTREAM_BASIC_VSTREAM_HPP_


#include <memory>
#include <string>
#include <unordered_map>

#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>


namespace meevax {


template <typename C> class visual_node;
template <typename C> using visual_edge = std::unique_ptr<meevax::visual_node<C>>;


template <typename C>
class visual_node
  : private std::shared_ptr<Display>,
    private std::unique_ptr<cairo_t, decltype(&cairo_destroy)>,
    public std::unordered_map<std::basic_string<C>, meevax::visual_edge<C>>
{
  static constexpr std::size_t default_window_width  {1280};
  static constexpr std::size_t default_window_height { 720};

#ifndef NDEBUG
  static constexpr std::size_t default_border_width  {   1};
#else
  static constexpr std::size_t default_border_width  {   0};
#endif

public:
  visual_node(const std::basic_string<C>& display_name = {""})
    : std::shared_ptr<Display> {XOpenDisplay(display_name.c_str()), XCloseDisplay},
      std::unique_ptr<cairo_t, decltype(&cairo_destroy)> {create(static_cast<Display*>(*this), XDefaultRootWindow(static_cast<Display*>(*this))), cairo_destroy}
  {
    if (static_cast<Display*>(*this) == nullptr)
    {
      std::cerr << "[error] XOpenDisplay(3) - failed to open display " << display_name << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }

  explicit visual_node(const meevax::visual_node<C>& node)
    : std::shared_ptr<Display> {node},
      std::unique_ptr<cairo_t, decltype(&cairo_destroy)> {create(static_cast<Display*>(*this), static_cast<Window>(node)), cairo_destroy}
  {}

  auto& operator[](std::basic_string<C>&& node_name)
  {
    if ((*this).find(node_name) == (*this).end())
    {
      meevax::visual_edge<C> edge {new meevax::visual_node<C> {*this}};
      (*this).emplace(node_name, std::move(edge));
    }

    return *(*this).at(node_name);
  }

  auto& operator()(std::size_t&& x, std::size_t&& y) const
  {
    XMoveWindow(static_cast<Display*>(*this), static_cast<Window>(*this), std::forward<decltype(x)>(x), std::forward<decltype(y)>(y));
    return *this;
  }

  auto& next_event() // XXX UGLY CODE
  {
    static XEvent event {};
    XNextEvent(static_cast<Display*>(*this), &event);
    return event;
  }

public:
  explicit operator Display*() const noexcept
  {
    return std::shared_ptr<Display>::get();
  }

  explicit operator cairo_t*() const noexcept
  {
    return std::unique_ptr<cairo_t, decltype(&cairo_destroy)>::get();
  }

  explicit operator cairo_surface_t*() const noexcept
  {
    return cairo_get_target(static_cast<cairo_t*>(*this));
  }

  explicit operator Window() const noexcept
  {
    return cairo_xlib_surface_get_drawable(static_cast<cairo_surface_t*>(*this));
  }

protected:
  static auto create(Display* display, const Window& window)
  {
    auto simple_window {XCreateSimpleWindow(display, window, 0, 0, default_window_width, default_window_height, default_border_width, XBlackPixel(display, XDefaultScreen(display)), XWhitePixel(display, XDefaultScreen(display)))};
    auto cairo_surface {cairo_xlib_surface_create(display, simple_window, XDefaultVisual(display, XDefaultScreen(display)), default_window_width, default_window_height)};

    XSelectInput(display, simple_window, ExposureMask | KeyPressMask); // TODO functionize

    return cairo_create(cairo_surface);
  }
};


} // namespace meevax


template <typename C, typename F>
auto& operator<<(const meevax::visual_node<C>& node, F&& f)
{
  return f(node);
}


template <typename C>
auto& operator<<(const meevax::visual_node<C>& cairo, C* c_str)
{
  cairo_show_text(static_cast<cairo_t*>(cairo), c_str);
  return cairo;
};


template <typename C>
auto& operator<<(const meevax::visual_node<C>& cairo, const C* c_str)
{
  cairo_show_text(static_cast<cairo_t*>(cairo), c_str);
  return cairo;
};


template <typename C>
auto& operator<<(const meevax::visual_node<C>& cairo, const std::basic_string<C>& text)
{
  cairo_show_text(static_cast<cairo_t*>(cairo), text.c_str());
  return cairo;
};


#endif
