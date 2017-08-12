#ifndef INCLUDED_MEEVAX_VSTREAM_BASIC_XLIB_VSTREAM_HPP_
#define INCLUDED_MEEVAX_VSTREAM_BASIC_XLIB_VSTREAM_HPP_


#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <xcb/xcb.h>
#include <cairo/cairo-xcb.h>

#include <meevax/cairo/surface.hpp>


namespace meevax {


template <typename C>
class basic_xcb_vstream
  : public meevax::cairo::surface
{
  std::vector<std::basic_string<C>> data_;

public:
  template <typename... Ts>
  explicit basic_xcb_vstream(Ts&&... args)
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
      (*this).connection.get(), (*this).id,
      XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
      std::vector<std::uint32_t> {width, height}.data()
    );

    cairo_xcb_surface_set_size((*this).get(), width, height);
  }
};


template <typename C>
using basic_vstream = meevax::basic_xcb_vstream<C>;


} // namespace meevax


#endif

