#ifndef INCLUDED_MEEVAX_STRING_RUNTIME_TYPENAME_HPP_
#define INCLUDED_MEEVAX_STRING_RUNTIME_TYPENAME_HPP_


#include <cstdlib>
#include <memory>
#include <string>
#include <typeinfo>

#include <cxxabi.h>


namespace meevax::string {


template <typename T>
std::string runtime_typename(const T& object)
{
  int status {0};

  std::unique_ptr<char, decltype(&std::free)> uptr {
    abi::__cxa_demangle(typeid(object).name(), nullptr, nullptr, &status),
    [](void* ptr) noexcept { std::free(ptr); }
  };

  return {status != 0 ? typeid(object).name() : uptr.get()};
}


} // namespace meevax::string


#endif
