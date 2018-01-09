#include <exception>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <dlfcn.h>
#include <sys/wait.h>
#include <unistd.h>

#include <boost/asio.hpp>
#include <boost/cstdlib.hpp>

#include <meevax/configure/version.hpp>
#include <meevax/posix/termios.hpp>
#include <meevax/posix/winsize.hpp>
#include <meevax/string/static_concat.hpp>

#include <meevax/semantics/r.hpp>
#include <meevax/semantics/w.hpp>


auto* dynamic_compile(const std::string& source_path, const std::string& so_path, const std::string& code)
{
  std::fstream source_code {source_path, std::ios_base::out | std::ios_base::trunc};
  source_code << code << "\n";
  source_code.close();

  using meevax::string::static_concat;

  switch (const auto pid {::fork()})
  {
  case 0:
    execvp(
      ::getenv("CXX"),
      std::vector<char*>
      {
        ::getenv("CXX"),
        static_concat("-std=c++17").data(),
        static_concat("-shared").data(),
        static_concat("-fpic").data(),
        static_concat("-O3").data(),
        static_concat("-o").data(),
        const_cast<char*>(    so_path.c_str()),
        const_cast<char*>(source_path.c_str()),
        nullptr
      }.data()
    );
    [[fallthrough]];

  case -1:
    throw std::system_error {errno, std::system_category()};

  default:
    for (auto status {0}; waitpid(pid, &status, WUNTRACED), !WIFEXITED(status) && !WIFSIGNALED(status); );
    break;
  }

  auto* handle {::dlopen(so_path.c_str(), RTLD_LAZY)};

  assert(handle != nullptr);

  return handle;
}


auto main(int argc, char** argv) -> int try
{
  const std::vector<std::string> args {argv, argv + argc};

  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  for (auto iter {std::begin(args) + 1}; iter != std::end(args); ++iter) [&]()
  {
    for (const auto& option : decltype(args) {"^-v$", "^--version$"})
    {
      if (std::regex_match(*iter, std::regex {option}))
      {
        std::cout << meevax::project_name << " - version " << meevax::version;
        std::cout << " (Compiled " << meevax::build_time << " on " << meevax::build_type << " build)\n";

        std::exit(boost::exit_success);
      }
    }

    for (const auto& option : decltype(args) {"^-h$", "^--help$"})
    {
      if (std::regex_match(*iter, std::regex {option}))
      {
        std::cout << meevax::project_name << " - version " << meevax::version;
        std::cout << " (Compiled " << meevax::build_time << " on " << meevax::build_type << " build)\n";

        std::cout << "\n";
        std::cout << "usage: " << meevax::project_name << " [options]\n";
        std::cout << "       based on GNU command line interfaces standards\n";
        std::cout << "\n";
        std::cout << "options:\n";
        std::cout << "  -h, --help     display this help and exit\n";
        std::cout << "  -v, --version  output version information and exit\n";

        std::exit(boost::exit_success);
      }
    }

    std::cerr << "[error] unknown option \"" << *iter << "\"\n";
    std::cerr << "        more information with: \"" << meevax::project_name << " --help\"\n";

    std::exit(boost::exit_failure);
  }();

  static meevax::posix::termios termios {STDIN_FILENO};
  termios.change_to_noncanonical_mode();

  // static meevax::posix::winsize winsize {STDIN_FILENO};

  for (std::string buffer {""}; buffer.push_back(meevax::semantics::r_<char>()), true; )
  {
    for (const auto& each : buffer)
    {
      if (std::isgraph(each))
      {
        std::stringstream sstream {};

        sstream << "#include <meevax/semantics/semiosis.hpp>\n"
                << "\n"
                << "extern \"C\" {\n"
                << "  meevax::semantics::semiosis<\'" << each << "\', char> " << each << " {};\n"
                << "}\n";

        auto* handle {dynamic_compile(
          std::string {"/tmp/mvx."} + std::string {each} + std::string {".char.cpp"},
          std::string {"/tmp/mvx."} + std::string {each} + std::string {".char.so"},
          sstream.str()
        )};

        auto* operation {
          ::dlsym(handle, std::string {each}.c_str())
        };

        reinterpret_cast<meevax::semantics::semiosis_base*>(operation)->operator()();
      }
    }
  }

  return boost::exit_success;
}

catch (const std::system_error& error)
{
  const auto code {error.code()};
  std::cerr << "\n[error] code: " << code.value() << " - " << code.message() << "\n";
  std::exit(error.code().value());
}

catch (const std::exception& error)
{
  std::cerr << "\n[error] " << error.what() << "\n";
  std::exit(boost::exit_exception_failure);
}

