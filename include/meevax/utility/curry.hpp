#ifndef INCLUDED_MEEVAX_UTILITY_CURRY_HPP
#define INCLUDED_MEEVAX_UTILITY_CURRY_HPP


/**
* @file curry.hpp
*
* 関数のカリー化関数を提供するヘッダ。
*/


#include <utility>


namespace meevax::utility
{
  /**
  * 関数のカリー化。それ以上でも以下でもない。
  */
  template <typename Function>
  constexpr auto curry(Function&& function)
  {
    return [&](auto&&... args)
    {
      return [&](auto&&... args_) -> decltype(auto)
      {
        return function(std::forward<decltype(args)>(args)..., std::forward<decltype(args_)>(args_)...);
      };
    };
  }
}; // meevax::utility


#endif // INCLUDED_MEEVAX_UTILITY_CURRY_HPP

