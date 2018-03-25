#ifndef INCLUDED_MEEVAX_UTILITY_NONCOPYABLE_HPP
#define INCLUDED_MEEVAX_UTILITY_NONCOPYABLE_HPP


#if __cplusplus < 201703L
namespace meevax {
namespace utility {
#else
namespace meevax::utility {
#endif


template <typename T>
class noncopyable
{
public:
  noncopyable(const noncopyable&) = delete;

  T& operator=(const T&) = delete;

protected:
   noncopyable() = default;
  ~noncopyable() = default;
};


#if __cplusplus < 201703L
} // namespace utility
} // namespace meevax
#else
} // namespace meevax::utility
#endif


#endif

