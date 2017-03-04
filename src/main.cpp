#include <iostream>
#include <string>
#include <vector>

#include <meevax/trial/arguments_parser.hpp>
#include <meevax/trial/constexpr_string.hpp>


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

  meevax::arguments_parser parser {std::move(argv_)};

  constexpr meevax::string hoge {"hogehoge"};

  static_assert(hoge.size() == 8, "ERROR!");

  return 0;
}
