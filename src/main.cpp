#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <cerrno>
#include <cstring>

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
      : fd_input {::open(device.c_str(), O_RDONLY)},
        fd_output {}
    {}

    virtual ~Neuron()
    {
      if (fd_input != -1) if (close(fd_input) != 0) std::cerr << "[error] " << strerror(errno) << std::endl;
      if (fd_output != -1) if (close(fd_output) != 0) std::cerr << "[error] " << strerror(errno) << std::endl;
    }

  private:
    void connect_from_(const std::string& device) { fd_input = open(device.c_str(), O_RDONLY); }
    void connect_to_(const std::string& device) { fd_output = open(device.c_str(), O_WRONLY); }

    virtual void read() {}
    virtual void write() {}
  };

  class BasicNeuron : public Neuron {
    static constexpr size_t buf_size {8};
    static constexpr unsigned int seconds {1};
    char buffer[buf_size];
  public:
    BasicNeuron(const std::string& device)
      : Neuron {device},
        buffer {"init"}
    {
      while (1) read();
    }

  private:
    void read() override
    {
      if (::read(fd_input, static_cast<void*>(buffer), buf_size) > 0) write();
      sleep(seconds);
    }

    void write() override
    {
      ::write(fd_output, static_cast<void*>(buffer), buf_size);
    }
  };
}

int main(int argc, char** argv)
{
  meevax::BasicNeuron neuron {"/dev/stdin"};

  return 0;
}
