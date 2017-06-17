#ifndef INCLUDED_MEEVAX_VSTREAM_BASIC_VSTREAM_HPP_
#define INCLUDED_MEEVAX_VSTREAM_BASIC_VSTREAM_HPP_


#include <string>
#include <unordered_map>
#include <memory>

#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>


namespace meevax {

template <typename C>
class basic_vstream
{
  std::unique_ptr<Display, decltype(&XCloseDisplay)> display_;

  std::unordered_map<
    std::string,
    std::unique_ptr<cairo_t, decltype(&cairo_destroy)>
  > surfaces_;

public:
  basic_vstream(const std::string& name = {""});
};

} // namespace meevax


template <typename C>
meevax::basic_vstream<C>::basic_vstream(const std::basic_string<C>& name)
  : display_ {XOpenDisplay(name.c_str()), XCloseDisplay},
    surfaces_ {}
{
  if (display_ == nullptr)
  {
    std::cerr << "[error] XOpenDisplay(3) - failed to open display " << name << std::endl;
    std::exit(EXIT_FAILURE);
  }
}


#endif
