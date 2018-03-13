#ifndef INCLUDED_MEEVAX_LAMBDA_OVERLOAD_HPP
#define INCLUDED_MEEVAX_LAMBDA_OVERLOAD_HPP


#include <utility>


namespace meevax::lambda {


template <typename... Ts>
class overloaded_lambdas;


template <typename T>
class overloaded_lambdas<T>
  : public T
{
public:
  overloaded_lambdas(T&& lambda)
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
  overloaded_lambdas(T&& lambda, U&& arg, Ts&&... args)
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


} // namespace meevax::lambda


#endif


// TODO これが動くか検証すること
//
// template <typename... Ts>
// struct overloaded_lambdas
//   public: Ts...
// {
//   using Ts::operator()...;
// };
//
// template <typename... Ts_>
// overloaded_lambdas(Ts_&&...)
//   -> overloaded_lambdas<Ts_...>;
//
// int main()
// {
//   overloaded_lambdas operation {
//     [](Hoge&) { hoge },
//     [](Fuga&) { fuga },
//   };
// }

