#ifndef INCLUDED_MEEVAX_XCB_ITERATOR_HPP_
#define INCLUDED_MEEVAX_XCB_ITERATOR_HPP_


#include <iterator>
#include <type_traits>

#include <xcb/xcb.h>


namespace meevax::xcb {


template <typename T, typename XcbIterator, typename NextFunctor, NextFunctor Next>
class iterator
  : public std::iterator<std::forward_iterator_tag, T>,
    public XcbIterator
{
public:
  explicit iterator(const XcbIterator& iter)
    : XcbIterator {iter}
  {}

  explicit iterator(const xcb_generic_iterator& iter)
    : XcbIterator {reinterpret_cast<const XcbIterator&>(iter)}
  {}

public:
  template <typename U>
  auto operator==(const U& rhs)
    -> typename std::enable_if<std::is_convertible<U,XcbIterator>::value, bool>::type
  {
    return (*this).data == rhs.data;
  }

  template <typename U>
  auto operator!=(const U& rhs)
    -> typename std::enable_if<std::is_convertible<U,XcbIterator>::value, bool>::type
  {
    return (*this).data != rhs.data;
  }

  auto& operator++() noexcept
  {
    Next(this);
    return *this;
  }

  auto operator++(int) noexcept
  {
    typename std::remove_reference<decltype(*this)>::type copy {*this};
    Next(this);
    return copy;
  }

  auto& operator*() const noexcept
  {
    return *(*this).data;
  }

  auto operator->() const noexcept
  {
    return (*this).data;
  }
};


} // namespace meevax::xcb


#endif

