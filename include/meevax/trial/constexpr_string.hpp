#ifndef INCLUDED_MEEVAX_TRIAL_CONSTEXPR_STRING_HPP_
#define INCLUDED_MEEVAX_TRIAL_CONSTEXPR_STRING_HPP_


#include <string>


namespace meevax {


template <typename C, typename Tr = std::char_traits<C>>
class basic_string
{
  C const* data_;

public:
  using traits_type = Tr;
  using value_type = typename Tr::char_type;
  using size_type = std::size_t;

  using reference = value_type&;
  using const_reference = const value_type&;

  constexpr explicit basic_string(const C* data) noexcept
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

private:
  constexpr std::size_t size(const C* data) const noexcept
  {
    return *data ? size(++data) + 1 : 0;
  }
};


} // namespace meevax


#endif
