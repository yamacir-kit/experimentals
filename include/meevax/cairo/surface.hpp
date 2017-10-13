#ifndef INCLUDED_MEEVAX_CAIRO_SURFACE_HPP
#define INCLUDED_MEEVAX_CAIRO_SURFACE_HPP


#include <algorithm>
#include <memory>
#include <utility>
#include <string>

#ifndef NDEBUG
#include <iostream>
#endif

#include <xcb/xcb.h>
#include <cairo/cairo-xcb.h>

#include <meevax/xcb/window.hpp>
#include <meevax/xcb/accessor.hpp>
#include <meevax/xcb/iterator.hpp>


namespace meevax::cairo {


// TODO
// いくらなんでも初期化コードが汚すぎる
// いい具合に直す方法が思い浮かばないのでとりあえず動く現状で様子を見るが、
// 流石にこれはクソなのでいつか直すこと


class surface
  : public meevax::xcb::window,
    public std::shared_ptr<cairo_surface_t>
{
  const std::string node_name_;

  static constexpr auto default_window_width_  {std::max<int>(1, 320)};
  static constexpr auto default_window_height_ {std::max<int>(1, 190)};

public:
  explicit surface(const std::shared_ptr<xcb_connection_t>& connection)
    : meevax::xcb::window {
        connection,
        xcb::accessor<xcb_setup_t, xcb_screen_t> {xcb_get_setup(connection.get())}.begin()->root
      },
      std::shared_ptr<cairo_surface_t> {
        cairo_xcb_surface_create(
          xcb::window::connection.get(), xcb::window::id,
          root_visualtype(xcb::window::connection),
          default_window_width_, default_window_height_
        ),
        cairo_surface_destroy
      },
      node_name_ {"unnamed"}
  {
#ifndef NDEBUG
    std::cerr << "[debug] meevax::cairo::surface constructor - new root node created \""
              << node_name_ << "\"\n" << std::flush;
#endif
  }

  explicit surface(const meevax::cairo::surface& surface, const std::string& node_name)
    : meevax::xcb::window {surface.connection, surface.id},
      std::shared_ptr<cairo_surface_t> {
        cairo_xcb_surface_create(
          xcb::window::connection.get(), xcb::window::id,
          root_visualtype(xcb::window::connection),
          default_window_width_, default_window_height_
        ),
        cairo_surface_destroy
      },
      node_name_ {node_name}
  {
#ifndef NDEBUG
    std::cerr << "[debug] meevax::cairo::surface constructor - new node created \""
              << node_name_ << "\"\n" << std::flush;
#endif
  }

public:
  void map() const noexcept
  {
    xcb_map_window(
      meevax::xcb::window::connection.get(),
      meevax::xcb::window::id
    );
  }

  void unmap() const noexcept
  {
    xcb_unmap_window(
      meevax::xcb::window::connection.get(),
      meevax::xcb::window::id
    );
  }

  template <typename... Ts>
  decltype(auto) configure(Ts&&... args) const noexcept
  {
    return xcb_configure_window(
      meevax::xcb::window::connection.get(),
      meevax::xcb::window::id,
      std::forward<Ts>(args)...
    );
  }

  template <typename... Ts>
  decltype(auto) change_attributes(Ts&&... args) const noexcept
  {
    return xcb_change_window_attributes(
      meevax::xcb::window::connection.get(),
      meevax::xcb::window::id,
      std::forward<Ts>(args)...
    );
  }

protected:
  static auto root_visualtype(const std::shared_ptr<xcb_connection_t>& connection)
    -> xcb_visualtype_t*
  {
    for (const auto& screen : xcb::accessor<xcb_setup_t, xcb_screen_t> {
                                xcb_get_setup(connection.get())
                              })
    {
      for (const auto& depth : xcb::accessor<xcb_screen_t, xcb_depth_t> {&screen})
      {
        for (auto&& visualtype : xcb::accessor<xcb_depth_t, xcb_visualtype_t> {&depth})
        {
          if (screen.root_visual == visualtype.visual_id)
          {
            return &visualtype;
          }
        }
      }
    }

    return nullptr;
  }
};


} // namespace meevax::cairo


#endif

