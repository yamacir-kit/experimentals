#ifndef INCLUDED_MEEVAX_TRIAL_RUNTIME_TYPEINFO_HPP_
#define INCLUDED_MEEVAX_TRIAL_RUNTIME_TYPEINFO_HPP_


#include <memory>
#include <string>
#include <typeinfo>

#include <cstdlib>

#include <cxxabi.h>


namespace meevax {


template <typename T>
std::string runtime_typename(const T& object)
{
  int status {0};

  std::unique_ptr<char, decltype(&std::free)> uptr {
    abi::__cxa_demangle(typeid(object).name(), nullptr, nullptr, &status),
    [](void* ptr) noexcept { std::free(ptr); }
  };

  return uptr.get();
}


} // namespace meevax


#endif
