#ifndef INCLUDED_MEEVAX_CAIRO_SURFACE_HPP_
#define INCLUDED_MEEVAX_CAIRO_SURFACE_HPP_


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


class surface
  : public meevax::xcb::window,
    public std::shared_ptr<cairo_surface_t>
{
  const std::string node_name_;

public:
  explicit surface(const std::shared_ptr<xcb_connection_t>& connection)
    : meevax::xcb::window {
        connection,
        xcb::accessor<xcb_setup_t, xcb_screen_t> {
          xcb_get_setup(connection.get())
        }.begin()->root
      },
      std::shared_ptr<cairo_surface_t> {
        cairo_xcb_surface_create(
            xcb::window::connection.get(),
            xcb::window::id,
            root_visualtype(xcb::window::connection),
            std::max<int>(1, 0),
            std::max<int>(1, 0)
        ),
        cairo_surface_destroy
      },
      node_name_ {"unnamed"}
  {}

  explicit surface(const meevax::cairo::surface& surface, const std::string& node_name)
    : meevax::xcb::window {surface.connection, surface.id},
      std::shared_ptr<cairo_surface_t> {
        cairo_xcb_surface_create(
            xcb::window::connection.get(),
            xcb::window::id,
            root_visualtype(xcb::window::connection),
            std::max<int>(1, 0),
            std::max<int>(1, 0)
        ),
        cairo_surface_destroy
      },
      node_name_ {node_name}
  {
#ifndef NDEBUG
    std::cout << "[debug] meevax::cairo::surface - new node created \"" << node_name_ << "\"\n";
#endif
  }

private:
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

