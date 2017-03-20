#include <iostream>
#include <string>
#include <vector>

// #include <meevax/trial/arguments_parser.hpp>
// #include <meevax/trial/ansi_escape_seqence.hpp>


int main(int argc, char** argv)
{
  std::vector<std::string> argv_ {
    "./utilib",
    "-abc",
    "--help",
    "-n42",
    "-n", "42",
    "--long1=arg1",
    "--long2", "arg2",
    "-",
    "-p",
    "--piyo"
  };

  // meevax::arguments_parser parser {std::move(argv_)};

  return 0;
}
