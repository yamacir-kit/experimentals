#include <iostream>
#include <map>
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
    void connect(int fd, const std::string& device, int flags) { fd = open(device.c_str(), flags); }
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
        buf {"init"}
    {
      while (1) read();
    }

    // void connect(const std::string& device) override
    // {
    //   fd_output = open(device.c_str(), O_WRONLY);
    // }
    //
    // void connect(const Neuron& neuron) override
    // {
    // }

  private:
    void read() override
    {
      if (::read(fd_input, static_cast<void*>(buf), buf_size) > 0) write();
      sleep(seconds);
    }

    void write() override
    {
      ::write(fd_output, static_cast<void*>(buf), buf_size);
    }
  };
}

int main(int argc, char** argv)
{
  meevax::BasicNeuron neuron {"/dev/stdin"};
  // neuron.connect("/dev/stdout");

  return 0;
}
