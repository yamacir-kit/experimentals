#ifndef INCLUDED_MEEVAX_XCB_CONTAINER_HPP_
#define INCLUDED_MEEVAX_XCB_CONTAINER_HPP_


namespace meevax::xcb {


template <typename Container,
          typename Iterator,
          typename SizeFunctor, SizeFunctor Size,
          typename BeginFunctor, BeginFunctor Begin,
          typename EndFunctor, EndFunctor End>
class container
{
  const Container* container_;

public:
  using value_type = typename Iterator::value_type;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  using       iterator =       Iterator;
  using const_iterator = const Iterator;

  using       reference =       value_type&;
  using const_reference = const value_type&;

  using       pointer =       value_type*;
  using const_pointer = const value_type*;

public:
  explicit container(const Container* container)
    : container_ {container}
  {}

  // explicit container(const Container& container)
  //   : container_ {&container}
  // {}

  iterator begin() noexcept
  {
    return iterator {Begin(container_)};
  }

  iterator end() noexcept
  {
    return iterator {End(Begin(container_))};
  }

  const_iterator begin() const noexcept
  {
    return const_iterator {Begin(container_)};
  }

  const_iterator end() const noexcept
  {
    return const_iterator {End(Begin(container_))};
  }

  size_type size() const noexcept
  {
    return Size(container_);
  }

  bool empty() const noexcept
  {
    return Size(container_) == 0;
  }

  operator Container()
  {
    return *container_;
  }
};


} // namespace meevax::xcb


#endif

