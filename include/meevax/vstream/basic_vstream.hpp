#ifndef INCLUDED_MEEVAX_VSTREAM_BASIC_XLIB_VSTREAM_HPP_
#define INCLUDED_MEEVAX_VSTREAM_BASIC_XLIB_VSTREAM_HPP_


#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <meevax/cairo/surface.hpp>
#include <meevax/type_traits/has_function_call_operator.hpp>


namespace meevax {


template <typename Char>
class basic_vstream
  : public meevax::cairo::surface
{
  std::vector<std::basic_string<Char>> data_;

public:
  template <typename... Ts>
  explicit basic_vstream(Ts&&... args)
    : meevax::cairo::surface {std::forward<Ts>(args)...}
  {}

public:
  void map() const noexcept
  {
    xcb_map_window(
      meevax::xcb::window::connection.get(),
      meevax::xcb::window::id
    );
  }

  void resize(std::uint32_t width, std::uint32_t height) const noexcept
  {
    xcb_configure_window(
      meevax::xcb::window::connection.get(),
      meevax::xcb::window::id,
      XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
      std::vector<std::uint32_t> {width, height}.data()
    );

    cairo_xcb_surface_set_size(meevax::cairo::surface::get(), width, height);
  }
};


template <typename Char, typename Functor,
          typename = typename std::enable_if<meevax::has_function_call_operator<Functor, const meevax::basic_vstream<Char>&>::value>::type>
inline decltype(auto) operator<<(const meevax::basic_vstream<Char>& lhs, Functor& rhs)
{
  return rhs(lhs);
}


} // namespace meevax


#endif

