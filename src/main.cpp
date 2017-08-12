#include <iostream>
#include <system_error>

#include <meevax/configure/version.hpp>
#include <meevax/graph/labeled_tree.hpp>
#include <meevax/vstream/basic_vstream.hpp>


int main(int argc, char** argv) try
{
  std::cout << "[debug] boost version: " << boost_version.data() << "\n";
  std::cout << "[debug] cairo version: " << cairo_version_string() << "\n\n";

  const std::shared_ptr<xcb_connection_t> connection {
    xcb_connect(nullptr, nullptr), xcb_disconnect
  };

  meevax::graph::labeled_tree<
    std::string, meevax::basic_vstream<char>
  > master {connection};

  return 0;
}

catch (const std::exception& error)
{
  std::cerr << "[error] " << error.what() << std::endl;
  std::exit(EXIT_FAILURE);
}

catch (const std::system_error& error)
{
  std::cerr << "[error] code: " << error.code().value() << " - " << error.code().message() << std::endl;
  std::exit(error.code().value());
}

