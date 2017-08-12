#include <chrono>
#include <cstdlib>
#include <iostream>
#include <system_error>
#include <thread>

#include <meevax/graph/labeled_tree.hpp>

#include <meevax/vstream/basic_vstream.hpp>
#include <meevax/vstream/vstream_manipulator.hpp>
#include <meevax/vstream/vstream_operator.hpp>

#include <meevax/version.hpp>


int main(int argc, char** argv) try
{
  std::cout << "[debug] boost version: " << boost_version.data() << "\n";
  std::cout << "[debug] cairo version: " << cairo_version_string() << "\n\n";


  return 0;
}

catch (const std::exception& error)
{
  std::cerr << "[error] standard exception occurred - " << error.what() << std::endl;
  std::exit(EXIT_FAILURE);
}

catch (const std::system_error& error)
{
  std::cerr << "[error] system error occurred\n"
            << "        code: " << error.code().value() << " - " << error.code().message() << std::endl;
  std::exit(error.code().value());
}

