#ifndef INCLUDED_MEEVAX_TRIAL_CONSTEXPR_STRING_HPP_
#define INCLUDED_MEEVAX_TRIAL_CONSTEXPR_STRING_HPP_


#include <iostream>
#include <string>


namespace meevax {


template <typename C, typename Tr = std::char_traits<C>>
class constexpr_string
{
  C const* data_;

public:
  using traits_type = Tr;
  using value_type = typename Tr::char_type;
  using size_type = std::size_t;

  using reference = value_type&;
  using const_reference = const value_type&;

  constexpr explicit constexpr_string(const C* data) noexcept
    : data_ {data}
  {}

  constexpr operator bool() const noexcept
  {
    return size() != 0;
  }

  constexpr const C* data() const noexcept
  {
    return data_;
  }

  constexpr std::size_t size() const noexcept
  {
    return size(data_);
  }

  template <typename T>
  friend auto& operator<<(std::ostream&, const meevax::constexpr_string<T>&);

private:
  constexpr std::size_t size(const C* data) const noexcept
  {
    return *data ? size(++data) + 1 : 0;
  }
};


template <typename T>
auto& operator<<(std::ostream& lhs, const meevax::constexpr_string<T>& rhs)
{
  return lhs << rhs.data();
}


} // namespace meevax


#endif
