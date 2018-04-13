#ifndef INCLUDED_MEEVAX_UTILITY_EXPLICIT_FUNCTION_HPP
#define INCLUDED_MEEVAX_UTILITY_EXPLICIT_FUNCTION_HPP


#include <utility>

#include <meevax/type_traits/type_traits.hpp>


#ifndef __cpp_nested_namespace_definitions
namespace meevax { namespace utility
#else
namespace meevax::utility
#endif
{
  template <typename T, typename = typename std::enable_if<std::is_class<T>::value>::type>
  class explicit_function
    : public T
  {
  public:
    using T::operator();

    template <typename... Ts>
    auto operator()(Ts&&... args) = delete;
  };
}
#ifndef __cpp_nested_namespace_definitions
} // namespace meevax::utility
#endif


#endif // INCLUDED_MEEVAX_UTILITY_EXPLICIT_FUNCTION_HPP

