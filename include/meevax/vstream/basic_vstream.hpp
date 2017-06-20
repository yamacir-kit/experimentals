#ifndef INCLUDED_MEEVAX_VSTREAM_BASIC_VSTREAM_HPP_
#define INCLUDED_MEEVAX_VSTREAM_BASIC_VSTREAM_HPP_


#include <memory>
#include <string>
#include <unordered_map>

#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>


namespace meevax {


template <typename C>
class visual_node;


template <typename C>
class visual_node
  : protected std::unique_ptr<cairo_t, decltype(&cairo_destroy)>
{
  std::unordered_map<
    std::basic_string<C>,
    std::unique_ptr<meevax::visual_node<C>>
  > sub_nodes_;

  static constexpr std::size_t default_window_width  {1280};
  static constexpr std::size_t default_window_height { 720};
  static constexpr std::size_t default_border_width  {   1};

public:
  template <typename... Ts>
  explicit visual_node(Ts&&... args)
    : std::unique_ptr<cairo_t, decltype(&cairo_destroy)> {std::forward<Ts>(args)...},
      sub_nodes_ {}
  {}

  auto& operator[](const std::basic_string<C>& node_name)
  {
    if (sub_nodes_.find(node_name) == sub_nodes_.end())
    {
      std::unique_ptr<meevax::visual_node<C>> surface {
        new meevax::visual_node<C> {
          create(static_cast<Display*>(*this), static_cast<Window>(*this))
        }
      };

      sub_nodes_.emplace(node_name, std::move(surface));
    }

    return *sub_nodes_.at(node_name);
  }

public:
  explicit operator cairo_t*() const noexcept
  {
    return (*this).get();
  }

  explicit operator cairo_surface_t*() const noexcept
  {
    return cairo_get_target(static_cast<cairo_t*>(*this));
  }

  explicit operator Display*() const noexcept
  {
    return cairo_xlib_surface_get_display(static_cast<cairo_surface_t*>(*this));
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

    XSelectInput(display, simple_window, ExposureMask | KeyPressMask);

    return meevax::visual_node<C> {cairo_create(cairo_surface), cairo_destroy};
  }
};


template <typename C>
class basic_vstream
  : public std::unique_ptr<Display, decltype(&XCloseDisplay)>,
    public meevax::visual_node<C>
{
public:
  basic_vstream(const std::basic_string<C>& display_name = {""})
    : std::unique_ptr<Display, decltype(&XCloseDisplay)> {XOpenDisplay(display_name.c_str()), XCloseDisplay},
      meevax::visual_node<C> {meevax::visual_node<C>::create(*this, XDefaultRootWindow(*this))}
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

  auto next_event()
  {
    static XEvent event {};
    XNextEvent(*this, &event);
    return event;
  }

private:
  operator Display*() const noexcept
  {
    return std::unique_ptr<Display, decltype(&XCloseDisplay)>::get();
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
