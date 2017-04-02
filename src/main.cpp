#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>

#include <unistd.h>
#include <sys/param.h>
#include <sys/wait.h>

#include "meevax/cmake_config.hpp"
#include "meevax/unix/shell.hpp"


std::string pwd()
{
  std::unique_ptr<char> buffer {new char[MAXPATHLEN]};
  return getcwd(buffer.get(), MAXPATHLEN) ? std::string(buffer.get()) : std::string();
}


int main(int argc, char** argv)
{
  unix::shell<char> sh {argc, argv};

  return sh.run();
}
