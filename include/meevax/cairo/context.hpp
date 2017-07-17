#ifndef INCLUDED_MEEVAX_CAIRO_CONTEXT_HPP_
#define INCLUDED_MEEVAX_CAIRO_CONTEXT_HPP_


#include <memory>

#include <xcb/xcb.h>
#include <cairo/cairo-xcb.h>

#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>


namespace meevax::cairo::xcb {


// https://lists.cairographics.org/archives/cairo/2015-June/026336.html
static xcb_visualtype_t* find_visual(xcb_connection_t* c, xcb_visualid_t visual)
{
  xcb_screen_iterator_t screen_iter = xcb_setup_roots_iterator(xcb_get_setup(c));

  for (; screen_iter.rem; xcb_screen_next(&screen_iter)) {
    xcb_depth_iterator_t depth_iter = xcb_screen_allowed_depths_iterator(screen_iter.data);
    for (; depth_iter.rem; xcb_depth_next(&depth_iter)) {
      xcb_visualtype_iterator_t visual_iter = xcb_depth_visuals_iterator(depth_iter.data);
      for (; visual_iter.rem; xcb_visualtype_next(&visual_iter))
        if (visual == visual_iter.data->visual_id)
          return visual_iter.data;
    }
  }

  return nullptr;
}

class context
{
  std::shared_ptr<cairo_t> cairo_;
  const xcb_window_t window_;

public:
  explicit context(xcb_connection_t* connection, const xcb_window_t& parent)
    : cairo_ {nullptr, cairo_destroy},
      window_ {xcb_generate_id(connection)}
  {
    auto iter {xcb_setup_roots_iterator(xcb_get_setup(connection)).data};

    xcb_create_window(
      connection, 0, window_, parent, 0, 0, 0, 0, 1,
      XCB_WINDOW_CLASS_INPUT_OUTPUT, (*iter).root_visual, 0, nullptr
    );

    auto surface {cairo_xcb_surface_create(
      connection, window_, find_visual(connection, (*iter).root_visual), 0, 0
    )};

    cairo_.reset(cairo_create(surface), cairo_destroy);
  }
};


} // namespace meevax::cairo::xcb


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
};


} // namespace meevax::cairo::xlib


#endif

