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


int main(int argc, char** argv)
{
  unix::shell<char> sh {argc, argv};

  return sh.run();
}
