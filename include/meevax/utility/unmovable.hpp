#ifndef INCLUDED_MEEVAX_UTILITY_UNMOVABLE_HPP
#define INCLUDED_MEEVAX_UTILITY_UNMOVABLE_HPP


#if __cplusplus < 201703L
namespace meevax {
namespace utility {
#else
namespace meevax::utility {
#endif


template <typename T>
class unmovable
{
public:
  unmovable(unmovable&&) = delete;

  T& operator=(T&&) = delete;

protected:
   unmovable() = default;
  ~unmovable() = default;
};


#if __cplusplus < 201703L
} // namespace utility
} // namespace meevax
#else
} // namespace meevax::utility
#endif


#endif

