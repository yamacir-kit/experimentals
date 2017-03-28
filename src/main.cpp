#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>

#include <unistd.h>
#include <sys/param.h>
#include <sys/wait.h>


std::string pwd()
{
  std::unique_ptr<char> buffer {new char[MAXPATHLEN]};
  return getcwd(buffer.get(), MAXPATHLEN) ? std::string(buffer.get()) : std::string();
}


auto split_arguments(const std::vector<std::string>& argv) // UGLY CODE
{
  std::vector<std::string> argv_ {argv};
  std::vector<std::string> args {};

  for (auto&& v : argv_)
  {
    if (v[0] == '-' && v[1] != '-')
    {
      v.erase(0, 1);

      while (std::isalpha(v[0]))
      {
        // args.emplace_back(&v[0], &v[1]);
        args.push_back({'-', v[0]});
        v.erase(0, 1);
      }
    }

    if (v[0] == '-' && v[1] == '-')
    {
      v.erase(0, 2);

      if (v.find_first_of('=') != std::string::npos)
      {
        // args.emplace_back(&v[0], &v[v.find('=')]);
        args.push_back("--" + std::string{&v[0], &v[v.find('=')]});
        v.erase(0, v.find('=') + 1);

        args.emplace_back(v.begin(), v.end());
      }

      else
      {
        // args.emplace_back(v.begin(), v.end());
        args.push_back("--" + std::string{v.begin(), v.end()});
        v.clear();
      }
    }

    if (!v.empty()) args.emplace_back(v.begin(), v.end());
  }

  return args;
}


namespace meevax {


class shell
{
  const std::vector<std::string> argv_;
        std::vector<std::string> input_;

public:
  explicit shell(int argc, char** argv)
    : argv_ {split_arguments({argv, argv + argc})}
  {
    for (auto iter {argv_.begin() + 1}; iter != argv_.end(); ++iter)
    {
      bool matched {false};

      for (const auto& help : std::vector<std::string>{"-h", "--help"})
      {
        if (*iter == help ? matched = true : false)
        {
          std::cout << "meevax - the most modern guardian of CUI culture.\n"
                    << "\n"
                    << "version 0.0.0 alpha\n"
                    << "\n"
                    << "  Looks like a shell, but supports only cd and pwd commands.\n"
                    << "  In other words, this is useless.\n"
                    << "\n"
                    << "USAGE: meevax [options]\n"
                    << "\n"
                    << "  -h, --help     display this help\n"
                    << "  -v, --version  display version information\n";

          exit(EXIT_SUCCESS);
        }
      }

      for (const auto version : std::vector<std::string>{"-v", "--version"})
      {
        if (*iter == version ? matched = true : false)
        {
          std::cout << "version 0.0.0 alpha\n"
                    << "\n"
                    << "  Looks like a shell, but supports only cd and pwd commands.\n"
                    << "  In other words, this is useless.\n";

          exit(EXIT_SUCCESS);
        }
      }

      if (!matched)
      {
        std::cout << "[error] invalid option '" << *iter << "'\n"
                  << "[hints] try 'meevax --help' for more information.\n";

        exit(EXIT_FAILURE);
      }
    }

    std::cout << "meevax: " << pwd() << "$ ";
  }

  int exec()
  {
    for (std::string buffer; !std::getline(std::cin, buffer).eof(); input_.clear())
    {
      for (std::stringstream input {buffer};
           std::getline(input, buffer, ' ');
           input_.push_back(buffer));

      if (input_[0] == "exit")
      {
        std::cout << "[debug] exit called" << std::endl;
        return 0;
      }

      else if (input_[0] == "pwd")
      {
        std::cout << "[debug] pwd called" << std::endl;
        std::cout << pwd() << std::endl;
      }

      else if (input_[0] == "help")
      {
        std::cout << "[debug] help called" << std::endl;
        std::cout << "        hogehoge"    << std::endl;
      }

      else launch();

      std::cout << "meevax: " << pwd() << "$ ";
    }

    return 0;
  }

protected:
  void launch() const
  {
    switch (pid_t pid {::fork()})
    {
    case  0: // child process
      std::cout << "[debug] child process execution" << std::endl
                << "        command: ";
      for (const auto& i : input_) std::cout << i << (&i != &input_.back() ? ' ' : '\n');
      exit(EXIT_SUCCESS);
      break;

    case -1:
      std::cerr << "meevax: forking error" << std::endl;
      exit(EXIT_FAILURE);
      break;

    default: // parent process
      std::cout << "[debug] parent process execution" << std::endl
                << "        pid: " << pid             << std::endl;

      int status {};
      do { waitpid(pid, &status, WUNTRACED); } while (!WIFEXITED(status) && !WIFSIGNALED(status));

      break;
    }
  }
};


} // namespace meevax


int main(int argc, char** argv)
{
  meevax::shell shell {argc, argv};

  return shell.exec();
}
