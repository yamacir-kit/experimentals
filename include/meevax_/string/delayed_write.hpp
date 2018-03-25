#ifndef INCLUDED_MEEVAX_STRING_DELAYED_WRITE_HPP
#define INCLUDED_MEEVAX_STRING_DELAYED_WRITE_HPP


#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <sys/ioctl.h>
#include <unistd.h>


namespace meevax::string {


template <typename C>
void delayed_write(const std::basic_stringstream<C>& sstream)
{
  for (const auto& buffer : sstream.str())
  {
    std::cout << buffer << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}


template <typename C>
void delayed_incremental_write(const std::basic_stringstream<C>& sstream)
{
  std::vector<std::pair<C,C>> text {};
  for (const auto& c : sstream.str()) { text.emplace_back(std::isprint(c) ? ' ' : c, c); }

  std::cout << "\e[?7l" << "\e[?25l" << std::flush;

  for (auto iter1 {text.begin()}; ; )
  {
    bool completed {true};
    std::pair<decltype(winsize::ws_row), decltype(winsize::ws_col)> cursor {0, 0};

    for (auto iter2 {text.begin()}; iter2 != iter1; ++iter2)
    {
      if ((*iter2).first != (*iter2).second)
      {
        std::cout << "\e[0;38;5;059m" << (*iter2).first++ << "\e[0m" << std::flush;
        std::this_thread::sleep_for(std::chrono::microseconds(500));
        completed = false;
      }

      else
      {
        std::cout << (*iter2).second << std::flush;
        if ((*iter2).second == '\n') { cursor.first++; }
      }
    }

    if (completed && iter1 == text.end()) { break; }

    else if (iter1 != text.begin())
    {
      if (cursor.first > 0) { std::cout << "\e[" << cursor.first << "A"; }
      std::cout << "\r" << std::flush;
    }

    if (iter1 != text.end()) { ++iter1; }
  }

  std::cout << "\e[?7h" << "\e[?25h" << std::flush;
}


} // namespace meevax::string


#endif
