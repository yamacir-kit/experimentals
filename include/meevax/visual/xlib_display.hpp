#ifndef INCLUDED_MEEVAX_VISUAL_XLIB_DISPLAY_HPP
#define INCLUDED_MEEVAX_VISUAL_XLIB_DISPLAY_HPP


#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#include <X11/Xlib.h>


namespace meevax {


class xlib_display
{
  std::unique_ptr<Display, decltype(&XCloseDisplay)> display_;

public:
  xlib_display(const std::string& name = {""})
    : display_ {XOpenDisplay(name.c_str()), XCloseDisplay}
  {
    if (display_ == nullptr)
    {
      std::cerr << "[error] XOpenDisplay(3) - failed to open display\n";
      std::exit(EXIT_FAILURE);
    }
  }

  operator Display*() const noexcept
  {
    return display_.get();
  }
};


} // namespace meevax


#endif
