#ifndef INCLUDED_MEEVAX_POSIX_INLINE_CURSES_HPP
#define INCLUDED_MEEVAX_POSIX_INLINE_CURSES_HPP


#include <list>
#include <string>
#include <system_error>

#include <sys/ioctl.h>
#include <unistd.h>


#if __cplusplus < 201703L
namespace meevax {
namespace posix {
#else
namespace meevax::posix {
#endif



class inline_curses
  : public ::winsize
{
  int fd_;

public:
  std::list<std::string> header, body, footer, control_line;

  std::string restore;

  explicit inline_curses(int fd)
    : fd_ {::isatty(fd) ? fd : throw std::system_error {errno, std::system_category()}},
      restore {""}
  {
    update();
  }

  const auto& assemble() const
  {
    static std::list<std::string> assembled {""};

    // auto reload = [&]()
    // {
    // }();

    for (const auto& line : header)
    {
      assembled.back() += line;
    }

    return assembled;
  }

private:
  void update() noexcept // TODO SIGNAL HANDLER
  {
    ::ioctl(fd_, TIOCGWINSZ, this);
  }
};


#if __cplusplus < 201703L
} // namespace posix
} // namespace meevax
#else
} // namespace meevax::posix
#endif


  // auto digits = [](unsigned int num)
  // {
  //   return num > 0 ? static_cast<int>(std::log(static_cast<double>(num))) + 1 : 1;
  // };
  //
  // std::cout << std::setw(digits(buffer.size())) << std::right
  //           << std::distance(std::begin(buffer), iter)
  //           << *iter << "\n";


  // while (true)
  // {
  //   icurses.header.clear();
  //
  //   if (icurses.header.empty())
  //   {
  //     icurses.header.emplace(std::end(icurses.header), "");
  //   }
  //
  //   icurses.header.back() += meevax::ansi_escape_sequence::color::foreground::green;
  //   icurses.header.back() += "meevax@";
  //   icurses.header.back() += boost::asio::ip::host_name();
  //   icurses.header.back() += ": ";
  //   icurses.header.back() += meevax::ansi_escape_sequence::color::foreground::yellow;
  //   icurses.header.back() += std::experimental::filesystem::current_path().string();
  //   icurses.header.back() += meevax::ansi_escape_sequence::color::foreground::white;
  //   icurses.header.back() += "$ ";
  //   icurses.header.back() += meevax::ansi_escape_sequence::attributes::off;
  //
  //   std::cout << icurses.restore;
  //   // std::cout << icurses.assemble();
  //
  //   const auto input {static_cast<char>(std::getchar())};
  //
  //   if (input == '\n')
  //   {
  //     icurses.body.emplace(std::end(icurses.body), "");
  //   }
  //   else
  //   {
  //     icurses.body.back() += input;
  //   }
  // }


#endif

