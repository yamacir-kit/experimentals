#ifndef INCLUDED_MEEVAX_XCB_ACCESSOR_HPP_
#define INCLUDED_MEEVAX_XCB_ACCESSOR_HPP_


#include <iterator>
#include <utility>

#include <meevax/xcb/accessor_traits.hpp>
#include <meevax/xcb/iterator.hpp>


namespace meevax::xcb {


template <typename T, typename U, typename AccessorTraits = meevax::xcb::accessor_traits<T,U>>
class accessor
{
  const T* data_;

public:
  using size_type = std::size_t;

  using       iterator =       typename AccessorTraits::iterator;
  using const_iterator = const typename AccessorTraits::iterator;

public:
  explicit accessor(const T* data)
    : data_ {data}
  {}

  iterator begin() noexcept
  {
    return iterator {
      AccessorTraits::begin(std::forward<decltype(data_)>(data_))
    };
  }

  iterator end() noexcept
  {
    return iterator {
      AccessorTraits::end(std::forward<decltype(data_)>(data_))
    };
  }

  const_iterator begin() const noexcept
  {
    return const_iterator {
      AccessorTraits::begin(std::forward<decltype(data_)>(data_))
    };
  }

  const_iterator end() const noexcept
  {
    return const_iterator {
      AccessorTraits::end(std::forward<decltype(data_)>(data_))
    };
  }

  size_type size() const noexcept
  {
    return AccessorTraits::size(data_);
  }

  bool empty() const noexcept
  {
    return size() == 0;
  }

  operator T() noexcept
  {
    return *data_;
  }
};


} // namespace meevax::xcb


#endif

