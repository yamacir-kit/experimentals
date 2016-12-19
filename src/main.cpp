#include <iostream>
#include <memory>
#include <map>
#include <vector>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace meevax
{
  class Neuron {
  public:
    int fd_input, fd_output;
    Neuron(const std::string& device)
      : fd_input {open(device.c_str(), O_RDONLY)},
        fd_output {}
    {}
  private:
    virtual void read() {}
    virtual void write() {}
  };

  class BasicNeuron : public Neuron {
    static constexpr size_t buf_size {8};
    static constexpr unsigned int seconds {1};
    char buf[buf_size];
  public:
    BasicNeuron(const std::string& device)
      : Neuron {device},
        buf {"hoge"}
    {
      while (1) read();
    }

  private:
    void read() override
    {
      if (::read(fd_input, static_cast<void*>(buf), buf_size) > 0) write();
      sleep(seconds);
    }

    void write() override { std::cout << buf << std::endl; }
  };
}

int main(int argc, char** argv)
{
  meevax::BasicNeuron neuron {"/dev/stdin"};

  return 0;
}
