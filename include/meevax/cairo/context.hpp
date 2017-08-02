#ifndef INCLUDED_MEEVAX_CAIRO_CONTEXT_HPP_
#define INCLUDED_MEEVAX_CAIRO_CONTEXT_HPP_


#include <memory>
#include <utility>

#include <xcb/xcb.h>
#include <cairo/cairo-xcb.h>

#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>

#include <meevax/xcb/window.hpp>
#include <meevax/xcb/accessor.hpp>
#include <meevax/xcb/iterator.hpp>

#include <meevax/utility/renamed_pair.hpp>


namespace meevax::cairo {


class surface
  : public meevax::xcb::window
{
  std::shared_ptr<cairo_surface_t> surface_;

public:
  explicit surface(std::shared_ptr<xcb_connection_t>& connection)
    : meevax::xcb::window {
        connection,
        xcb::accessor<xcb_setup_t, xcb_screen_t> {
          xcb_get_setup(connection.get())
        }.begin()->root
      }
  {
    surface_ = std::shared_ptr<cairo_surface_t> {
      cairo_xcb_surface_create(
          xcb::window::connection.get(),
          xcb::window::id,
          root_visualtype(xcb::window::connection),
          0, 0
      ),
      cairo_surface_destroy
    };
  }

  explicit surface(const meevax::cairo::surface& surface)
    : meevax::xcb::window {surface.connection, surface.id}
  {
    surface_ = std::shared_ptr<cairo_surface_t> {
      cairo_xcb_surface_create(
          xcb::window::connection.get(),
          xcb::window::id,
          root_visualtype(xcb::window::connection),
          0, 0
      ),
      cairo_surface_destroy
    };
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


namespace meevax::cairo::xlib {


class context
{
  std::shared_ptr<cairo_t> cairo_;

public:
  explicit context(Display* display, const Window& parent)
    : cairo_ {nullptr, cairo_destroy}
  {
    auto window {XCreateSimpleWindow(
      display, parent, 0, 0, 1280, 720, 1,
      XBlackPixel(display, XDefaultScreen(display)),
      XWhitePixel(display, XDefaultScreen(display))
    )};

    auto surface {cairo_xlib_surface_create(
      display, window, XDefaultVisual(display, XDefaultScreen(display)), 1, 1
    )};

    XSelectInput(display, window, ExposureMask | KeyPressMask);

    cairo_.reset(cairo_create(surface), cairo_destroy);
  };

public:
  explicit operator cairo_t*() const
  {
    return cairo_.get();
  }

  explicit operator cairo_surface_t*() const
  {
    return cairo_get_target(static_cast<cairo_t*>(*this));
  }

  explicit operator Display*() const
  {
    return cairo_xlib_surface_get_display(static_cast<cairo_surface_t*>(*this));
  }

  explicit operator Window() const
  {
    return cairo_xlib_surface_get_drawable(static_cast<cairo_surface_t*>(*this));
  }

public:
  template <typename C>
  auto& extents(const std::basic_string<C>& text) const
  {
    static cairo_text_extents_t extents {};
    cairo_text_extents(static_cast<cairo_t*>(*this), text.c_str(), &extents);
    return extents;
  }

  auto& point() const
  {
    static meevax::utility::renamed_pair::point<double> point {};
    cairo_get_current_point(static_cast<cairo_t*>(*this), &point.x, &point.y);
    return point;
  }
};


} // namespace meevax::cairo::xlib


#endif

