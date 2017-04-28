#ifndef INCLUDED_UTILIB_UNIX_FORK_HPP_
#define INCLUDED_UTILIB_UNIX_FORK_HPP_


#include <iostream>
#include <system_error>

#include <cerrno>
#include <cstdlib>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


namespace unix {


class fork
{
public:
  template <typename F>
  void operator()(F&& f) const noexcept(false)
  {
    switch (auto pid {::fork()})
    {
    case  0:
      try { f(); }

      catch (std::system_error& error)
      {
        std::cerr << "[error] code: " << error.code().value() << " - " << error.code().message() << std::endl;
        return;
      }

      catch (...) { throw; }

    case -1:
      throw std::system_error {errno, std::system_category()};

    default:
      int status_ {};

      do {
        ::waitpid(pid, &status_, WUNTRACED);
      } while (!WIFEXITED(status_) && !WIFSIGNALED(status_));
    }
  }
};


} // namespace unix


#endif
