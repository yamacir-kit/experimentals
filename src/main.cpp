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
      : fd_input {::open(device.c_str(), O_RDONLY)},
        fd_output {}
    {}
    virtual ~Neuron()
    {
      close(fd_input);
      close(fd_output);
    }
    virtual void connect(const std::string& device) {}
    virtual void connect(const Neuron& neuron) {}
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

    void connect(const std::string& device) override
    {
      fd_output = open(device.c_str(), O_WRONLY);
    }

    void connect(const Neuron& neuron) override
    {
    }

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
  neuron.connect("/dev/stdout");

  return 0;
}
