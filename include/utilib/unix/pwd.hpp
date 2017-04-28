#ifndef INCLUDED_UTILIB_UNIX_PWD_HPP_
#define INCLUDED_UTILIB_UNIX_PWD_HPP_


#include <memory>
#include <string>

#include <sys/param.h> // MAXPATHLEN
#include <unistd.h>


namespace unix {


template <typename C>
auto pwd()
  -> std::basic_string<C>
{
  std::unique_ptr<C> buffer {new C[MAXPATHLEN]};
  return {::getcwd(buffer.get(), MAXPATHLEN) ? buffer.get() : ""};
}


} // namespace unix


#endif
