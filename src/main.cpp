#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <cerrno>
#include <cstring>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace utilib
{
  class unique_fd {
    int fd_ = -1;

  public:
    constexpr unique_fd() noexcept = default;

    explicit unique_fd(int fd) noexcept
      : fd_ {fd}
    {}

    unique_fd(unique_fd&& u) noexcept
      : fd_ {u.release()}
    {}

    ~unique_fd() { if (fd_ != -1) close(fd_); }

    unique_fd& operator=(unique_fd&& u) noexcept = default;

    operator int() const noexcept { return fd_; }
    operator bool() const noexcept { return fd_ != -1; }

    int get() const noexcept { return fd_; }

    int release() noexcept
    {
      int tmp = fd_;
      fd_ = -1;
      return tmp;
    }

    void swap(unique_fd& u) noexcept { std::swap(fd_, u.fd_); }
    friend void swap(unique_fd& u, unique_fd& u2) noexcept { u.swap(u2); }

    void reset(int fd = -1) noexcept { unique_fd(fd).swap(*this); }

    friend int close(unique_fd& u) noexcept
    {
      int closed = ::close(u.fd_);
      u.fd_ = -1;
      return closed;
    }

    unique_fd(const unique_fd&) = delete;
    unique_fd& operator=(const unique_fd&) = delete;
  };
} /* namespace utilib */

namespace meevax
{
  class Neuron {
  public:
    utilib::unique_fd ifd, ofd;

    Neuron(const std::string& dev_input, const std::string& dev_output)
      : ifd {connect_from_(dev_input.c_str())},
        ofd {connect_to_(dev_output.c_str())}
    {}

    virtual ~Neuron() {}

  private:
    int connect_from_(const std::string& device) { return open(device.c_str(), O_RDONLY); }
    int connect_to_(const std::string& device) { return open(device.c_str(), O_WRONLY); }

    virtual void read() {}
    virtual void write() {}
  };

  class ParrotNeuron : public Neuron {
    static constexpr size_t buf_size {8};
    static constexpr unsigned int seconds {1};
    char buffer[buf_size];
  public:
    ParrotNeuron(const std::string& dev_input, const std::string& dev_output)
      : Neuron {dev_input, dev_output},
        buffer {"init"}
    {
      while (1) read();
    }

  private:
    void read() override
    {
      if (::read(ifd, static_cast<void*>(buffer), buf_size) > 0) write();
      sleep(seconds);
    }

    void write() override
    {
      ::write(ofd, static_cast<void*>(buffer), buf_size);
    }
  };

  class InputDevice {
    utilib::unique_fd fd_;
  public:
    constexpr InputDevice() noexcept = default;
    InputDevice(const std::string& dev) : fd_ {::open(dev.c_str(), O_RDONLY)} {}
    virtual void read() noexcept = 0;
  };

  class OutputDevice {
    utilib::unique_fd fd_;
  public:
    constexpr OutputDevice() noexcept = default;
    OutputDevice(const std::string& dev) : fd_ {::open(dev.c_str(), O_WRONLY)} {}
    virtual void write() noexcept = 0;
  };
} /* namespace meevax */

int main(int argc, char** argv)
{
  meevax::ParrotNeuron neuron {"/dev/stdin", "/dev/stdout"};

  return 0;
}
