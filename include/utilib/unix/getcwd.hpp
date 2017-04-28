#ifndef INCLUDED_UTILIB_UNIX_GETCWD_HPP_
#define INCLUDED_UTILIB_UNIX_GETCWD_HPP_


#include <memory>
#include <string>

#include <sys/param.h> // MAXPATHLEN
#include <unistd.h>


namespace unix {


template <typename C>
auto getcwd()
  -> std::basic_string<C>
{
  std::unique_ptr<C> buffer {new C[MAXPATHLEN]};
  return {::getcwd(buffer.get(), MAXPATHLEN) ? buffer.get() : ""};
}


} // namespace unix


#endif
