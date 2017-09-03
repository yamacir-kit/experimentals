#ifndef INCLUDED_MEEVAX_TYPE_TRAITS_HAS_VALUE_TYPE_HPP
#define INCLUDED_MEEVAX_TYPE_TRAITS_HAS_VALUE_TYPE_HPP


#include <type_traits>
#include <utility>


namespace meevax {


class has_value_type_
{
public:
  template <typename T, typename = typename T::value_type>
  static constexpr decltype(auto) check(T&) noexcept
  {
    return std::true_type {};
  }

  template <typename T>
  static constexpr decltype(auto) check(...) noexcept
  {
    return std::false_type {};
  }
};


template <typename T>
class has_value_type
  : public decltype(has_value_type_::check<T>(std::declval<T>()))
{};


} // namespace meevax


#endif

