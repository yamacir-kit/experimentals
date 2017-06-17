#ifndef INCLUDED_MEEVAX_VSTREAM_BASIC_VSTREAM_HPP_
#define INCLUDED_MEEVAX_VSTREAM_BASIC_VSTREAM_HPP_


#include <string>
#include <unordered_map>
#include <memory>

#include <X11/Xlib.h>


namespace meevax {

template <typename C>
class basic_vstream
{
  std::unique_ptr<Display,decltype(&XCloseDisplay)> display_;

public:
  basic_vstream(const std::string& name = {""});
};

} // namespace meevax


template <typename C>
meevax::basic_vstream<C>::basic_vstream(const std::string& name)
  : display_ {XOpenDisplay(name.c_str()), XCloseDisplay}
{
  if (display_ == nullptr)
  {
    std::cerr << "[error] XOpenDisplay(3) - failed to open display " << name << std::endl;
    std::exit(EXIT_FAILURE);
  }
}


#endif
