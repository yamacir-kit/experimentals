#ifndef INCLUDED_MEEVAX_VSTREAM_BASIC_VSTREAM_HPP_
#define INCLUDED_MEEVAX_VSTREAM_BASIC_VSTREAM_HPP_


#include <memory>
#include <unordered_map>
#include <utility>

#include <meevax/vstream/graphix_impl.hpp>


namespace meevax {


template <typename C> class visual_node;
template <typename C> using visual_edge = std::unique_ptr<meevax::visual_node<C>>;


template <typename C>
class visual_node
  : public meevax::graphix_impl,
    public std::unordered_map<std::basic_string<C>, meevax::visual_edge<C>>
{
public:
  // visual_node(Display* display)
  //   : std::unique_ptr<cairo_t, decltype(&cairo_destroy)> {create(display, XDefaultRootWindow(display)), cairo_destroy}
  // {}
  //
  // explicit visual_node(const meevax::visual_node<C>& node)
  //   : std::unique_ptr<cairo_t, decltype(&cairo_destroy)> {create(static_cast<Display*>(node), static_cast<Window>(node)), cairo_destroy}
  // {}

  template <typename... Ts>
  explicit visual_node(Ts&&... args)
    : meevax::graphix_impl {std::forward<Ts>(args)...}
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

//   [[deprecated]] auto& operator()(std::size_t&& x, std::size_t&& y) const
//   {
//     XMoveWindow(static_cast<Display*>(*this), static_cast<Window>(*this), std::forward<decltype(x)>(x), std::forward<decltype(y)>(y));
//     return *this;
//   }
//
//   auto& next_event() // XXX UGLY CODE
//   {
//     static XEvent event {};
//     XNextEvent(static_cast<Display*>(*this), &event);
//     return event;
//   }
//
// public: // XXX UGLY OPERATORS
//   explicit operator cairo_t*() const noexcept
//   {
//     return std::unique_ptr<cairo_t, decltype(&cairo_destroy)>::get();
//   }
//
//   explicit operator cairo_surface_t*() const noexcept
//   {
//     return cairo_get_target(static_cast<cairo_t*>(*this));
//   }
//
//   explicit operator Display*() const noexcept
//   {
//     return cairo_xlib_surface_get_display(static_cast<cairo_surface_t*>(*this));
//   }
//
//   explicit operator Window() const noexcept
//   {
//     return cairo_xlib_surface_get_drawable(static_cast<cairo_surface_t*>(*this));
//   }
//
// protected:
//   static auto create(Display* display, const Window& window)
//   {
//     static constexpr std::size_t default_window_width  {1280};
//     static constexpr std::size_t default_window_height { 720};
//
// #ifndef NDEBUG
//     static constexpr std::size_t default_border_width  {   1};
// #else
//     static constexpr std::size_t default_border_width  {   0};
// #endif
//
//     auto simple_window {XCreateSimpleWindow(display, window, 0, 0, default_window_width, default_window_height, default_border_width, XBlackPixel(display, XDefaultScreen(display)), XWhitePixel(display, XDefaultScreen(display)))};
//     auto cairo_surface {cairo_xlib_surface_create(display, simple_window, XDefaultVisual(display, XDefaultScreen(display)), default_window_width, default_window_height)};
//
//     XSelectInput(display, simple_window, ExposureMask | KeyPressMask); // TODO functionize
//
//     return cairo_create(cairo_surface);
//   }
};


} // namespace meevax


// template <typename C, typename F>
// auto& operator<<(const meevax::visual_node<C>& node, F&& f)
// {
//   return f(node);
// }
//
//
// template <typename C>
// auto& operator<<(const meevax::visual_node<C>& cairo, C* c_str)
// {
//   cairo_show_text(static_cast<cairo_t*>(cairo), c_str);
//   return cairo;
// };
//
//
// template <typename C>
// auto& operator<<(const meevax::visual_node<C>& cairo, const C* c_str)
// {
//   cairo_show_text(static_cast<cairo_t*>(cairo), c_str);
//   return cairo;
// };
//
//
// template <typename C>
// auto& operator<<(const meevax::visual_node<C>& cairo, const std::basic_string<C>& text)
// {
//   cairo_show_text(static_cast<cairo_t*>(cairo), text.c_str());
//   return cairo;
// };


#endif
