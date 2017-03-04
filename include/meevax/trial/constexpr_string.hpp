#ifndef INCLUDED_MEEVAX_TRIAL_CONSTEXPR_STRING_HPP_
#define INCLUDED_MEEVAX_TRIAL_CONSTEXPR_STRING_HPP_


namespace meevax {


class string
{
  char const* data_;

public:
  constexpr explicit string(const char* data) noexcept
    : data_ {data}
  {}

  constexpr operator char const*() const noexcept
  {
    return data_;
  }

  constexpr std::size_t size() const noexcept
  {
    return size(data_);
  }

private:
  constexpr std::size_t size(const char* data) const noexcept
  {
    return *data ? size(++data) + 1 : 0;
  }
};


} // namespace meevax


#endif
