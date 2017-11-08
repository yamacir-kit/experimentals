#ifndef INCLUDED_MEEVAX_LAMBDA_OVERLOAD_HPP
#define INCLUDED_MEEVAX_LAMBDA_OVERLOAD_HPP


#include <utility>


#if __cplusplus < 201703L
namespace meevax {
namespace lambda {
#else
namespace meevax::lambda {
#endif


template <typename... Ts>
class overloaded_lambdas;


template <typename T>
class overloaded_lambdas<T>
  : public T
{
public:
  constexpr overloaded_lambdas(T&& lambda)
    : T {std::forward<T>(lambda)}
  {}

  using T::operator();
};


template <typename T, typename U, typename... Ts>
class overloaded_lambdas<T, U, Ts...>
  : public T,
    public overloaded_lambdas<U, Ts...>
{
public:
  constexpr overloaded_lambdas(T&& lambda, U&& arg, Ts&&... args)
    : T {std::forward<T>(lambda)},
      overloaded_lambdas<U, Ts...> {std::forward<U>(arg), std::forward<Ts>(args)...}
  {}

  using T::operator();
  using overloaded_lambdas<U, Ts...>::operator();
};


template <typename... Ts>
constexpr auto overload(Ts&&... args)
  -> overloaded_lambdas<typename std::decay<Ts>::type...>
{
  return { std::forward<Ts>(args)... };
}


#if __cplusplus < 201703L
} // namespace lambda
} // namespace meevax
#else
} // namespace meevax::lambda
#endif


#endif

