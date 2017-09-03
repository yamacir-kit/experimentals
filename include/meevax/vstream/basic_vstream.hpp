#ifndef INCLUDED_MEEVAX_VSTREAM_BASIC_XLIB_VSTREAM_HPP
#define INCLUDED_MEEVAX_VSTREAM_BASIC_XLIB_VSTREAM_HPP


#include <algorithm>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <meevax/cairo/surface.hpp>
#include <meevax/type_traits/has_operator.hpp>


namespace meevax {


template <typename Char>
class basic_vstream
  : public meevax::cairo::surface
{
public:
  std::basic_string<Char> data;

  template <typename... Ts>
  explicit basic_vstream(Ts&&... args)
    : meevax::cairo::surface {std::forward<Ts>(args)...}
  {}

public:
  decltype(auto) read(Char c) noexcept(noexcept(data.push_back(std::declval<Char>())))
  {
    return data.push_back(c);
  }

public:
  void map() const noexcept
  {
    xcb_map_window(
      meevax::xcb::window::connection.get(),
      meevax::xcb::window::id
    );
  }

  [[deprecated]] void resize(std::uint32_t width, std::uint32_t height) const noexcept
  {
    xcb_configure_window(
      meevax::xcb::window::connection.get(),
      meevax::xcb::window::id,
      XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
      std::vector<std::uint32_t> {width, height}.data()
    );

    cairo_xcb_surface_set_size(meevax::cairo::surface::get(), width, height);
  }

  template <typename... Ts>
  decltype(auto) configure(Ts&&... args)
  {
    return xcb_configure_window(
      meevax::xcb::window::connection.get(),
      meevax::xcb::window::id,
      std::forward<Ts>(args)...
    );
  }

  template <typename... Ts>
  decltype(auto) change_attributes(Ts&&... args)
  {
    return xcb_change_window_attributes(
      meevax::xcb::window::connection.get(),
      meevax::xcb::window::id,
      std::forward<Ts>(args)...
    );
  }
};


template <typename T, typename Functor,
          typename = typename std::enable_if<meevax::has_function_call_operator<Functor, T&>::value>::type>
inline decltype(auto) operator<<(T& lhs, Functor&& rhs)
{
  return rhs(lhs);
}


template <typename T, typename Functor,
          typename = typename std::enable_if<meevax::has_function_call_operator<Functor, T&>::value>::type>
inline decltype(auto) operator>>(Functor& lhs, T&& rhs)
{
  return lhs(rhs);
}


} // namespace meevax


#endif

