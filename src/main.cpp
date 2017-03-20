#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>

#include <unistd.h>
#include <sys/param.h>
#include <sys/wait.h>


namespace meevax {


class meevax
{
  const std::vector<std::string> argv_;
        std::vector<std::string> input_;

public:
  meevax(int argc, char** argv)
    : argv_ {argv, argv + argc}
  {
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
  std::string pwd() const noexcept
  {
    std::unique_ptr<char> buffer {new char[MAXPATHLEN]};
    return getcwd(buffer.get(), MAXPATHLEN) ? std::string(buffer.get()) : std::string();
  }

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
  meevax::meevax shell {argc, argv};

  return shell.exec();
}
