#ifndef INCLUDED_MEEVAX_XCB_ACCESSOR_TRAITS_HPP_
#define INCLUDED_MEEVAX_XCB_ACCESSOR_TRAITS_HPP_


#include <utility>

#include <meevax/xcb/iterator.hpp>


namespace meevax::xcb {


template <typename T, typename U>
class accessor_traits
{};


template <>
class accessor_traits<xcb_setup_t, xcb_screen_t>
{
public:
  template <typename... Ts>
  static decltype(auto) begin(Ts&&... args)
  {
    return xcb_setup_roots_iterator(std::forward<Ts>(args)...);
  }

  template <typename... Ts>
  static decltype(auto) end(Ts&&... args)
  {
    return xcb_screen_end(begin(std::forward<Ts>(args)...));
  }

  template <typename... Ts>
  static decltype(auto) size(Ts&&... args)
  {
    return xcb_setup_roots_length(std::forward<Ts>(args)...);
  }

  using accessor = xcb_setup_t;
  using iterator = meevax::xcb::iterator<xcb_screen_t, xcb_screen_iterator_t, decltype(&xcb_screen_next), &xcb_screen_next>;
};


} // namespace meevax::xcb


#endif

