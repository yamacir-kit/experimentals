case   0:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + 2"                           << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case   1:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + A"                           << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case   2:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + B"                           << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case   3:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + C" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case   4:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + D" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case   5:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + E" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case   6:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + F" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case   7:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + G" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case   8:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + H" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case   9:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Tab, Ctrl + I" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  10:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Return, Ctrl + J"                   << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  line_buffer_.push_back(word_buffer_);
  word_buffer_.clear();

  text_buffer_.push_back(line_buffer_);
  line_buffer_.clear();

  ++cursor_.first;

  if (parse_unit_ == semantic_parse_unit::line)
  {
    std::cout << "[debug] start semantic parse\n";
    std::cout << "        target: line " << cursor_.first << " (";

    for (const auto& word : text_buffer_[cursor_.first - 1])
    {
      std::cout << word << (&word != &text_buffer_[cursor_.first - 1].back() ? " " : ")\n");
    }

    std::cout << "        syntax: shell\n";
    std::cout << "        parser: execvp(3)\n";
    std::cout << "        struct: std::vector<std::basic_string<char_type>>\n";
    std::cout << "\n";

    unix::fork()(unix::execvp<char_type>(text_buffer_[cursor_.first - 1]));

    std::cout << "\n";
  }

  break;

case  11:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + K" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  12:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + L" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  13:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + M" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  14:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + N" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  15:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + O" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  16:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + P" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  17:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + Q" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  18:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + R" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  19:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + S" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  20:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + T" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  21:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + U" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  22:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + V" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  23:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + W" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  24:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + X" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  25:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + Y" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  26:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + Z" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  27:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Escape, Ctrl + [" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  28:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + Space" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  29:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + ]" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  30:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + 6" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  31:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Ctrl + -" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  32:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: \e[0;33m" << __FILE__ << "\e[0;37m\n";
  std::cout << "        LINE: \e[0;36m" << __LINE__ << "\e[0;37m\n";
  std::cout << "        ANSI: \e[1;37m" << "Space"  << "\e[0;37m\n";
  std::cout << "        CODE: \e[0;36m" << static_cast<int>(char_buffer_);
  std::cout << "\n\e[0;37m\n";

  std::cout << "        TRUE: always\n";
  std::cout << "        THEN: line_buffer_.push_back(word_buffer_);\n";
  std::cout << "              word_buffer_.clear();\n";
  std::cout << "\n";
#endif
  line_buffer_.push_back(word_buffer_);
  word_buffer_.clear();
  break;

case  33:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + 1" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  34:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + Single Quote"               << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  35:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + 3" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  36:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + 4" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  37:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + 5" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  38:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + 7" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  39:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Single Quote"                       << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  40:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + 9" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  41:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + 0" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  42:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + 8" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  43:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + Equal"                      << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  44:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Comma"                              << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  45:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Hyphen"                             << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  46:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Dot"                                << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  47:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Slash"                              << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  48:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: 0" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  49:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: 1" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  50:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: 2" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  51:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: 3" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  52:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: 4" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  53:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: 5" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  54:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: 6" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  55:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: 7" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  56:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: 8" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  57:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: 9"                                  << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  58:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + Semi Colon"                 << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  59:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Semi Colon"                         << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  60:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + Comma"                      << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  61:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Equal"                              << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  62:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + Dot"                        << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  63:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + Slash"                      << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  64:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + 2" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  65:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + A"                          << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  66:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + B" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  67:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + C" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  68:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + D" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  69:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + E" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  70:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + F" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  71:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + G" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  72:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + H" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  73:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + I" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  74:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + J" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  75:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + K" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  76:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + L" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  77:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + M" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  78:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + N" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  79:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + O" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  80:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + P" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  81:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + Q" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  82:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + R" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  83:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + S" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  84:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + T" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  85:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + U" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  86:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + V" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  87:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + W" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  88:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + X" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  89:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + Y" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  90:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + Z" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  91:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Left Bracket" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  92:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Space" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  93:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Right Bracket" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  94:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + 6" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case  95:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + Hyphen"                     << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  96:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Back Quote"                         << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  97:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: A" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  98:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: B" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case  99:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: C" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 100:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: D" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 101:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: E" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 102:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: F" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 103:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: G" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 104:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: H" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 105:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: I" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 106:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: J" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 107:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: K" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 108:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: L" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 109:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: M" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 110:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: N" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 111:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: O" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 112:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: P" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 113:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Q" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 114:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: R" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 115:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: S" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 116:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: T" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 117:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: U" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 118:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: V" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 119:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: W" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 120:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: X" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 121:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Y" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 122:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Z" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 123:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + Left Bracket" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case 124:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + Space" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case 125:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + Right Bracket" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << static_cast<decltype(char_buffer_)>(char_buffer_);
#endif
  break;

case 126:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: Shift + Back Quote"                 << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#endif
  word_buffer_.push_back(char_buffer_);
  break;

case 127:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: \e[0;33m" << __FILE__    << "\e[0;37m\n";
  std::cout << "        LINE: \e[0;36m" << __LINE__    << "\e[0;37m\n";
  std::cout << "        ANSI: \e[1;37m" << "Backspace" << "\e[0;37m\n";
  std::cout << "        CODE: \e[0;36m" << static_cast<int>(char_buffer_);
  std::cout << "\n\e[0;37m\n";

  if (!word_buffer_.empty())
  {
    std::cout << "        TRUE: !word_buffer_.empty()\n";
    std::cout << "        THEN: word_buffer_.pop_back()\n";
    std::cout << "\n";

    word_buffer_.pop_back();
  }

  else if (!line_buffer_.empty())
  {
    std::cout << "        TRUE: !line_buffer_.empty()\n";
    std::cout << "        THEN: word_buffer_ = line_buffer_.back()\n";
    std::cout << "              line_buffer_.pop_back()\n";
    std::cout << "\n";

    word_buffer_ = line_buffer_.back();
    line_buffer_.pop_back();
  }

  else
  {
    std::cout << "        TRUE: word_buffer_.empty() && line_buffer_.empty()\n";
    std::cout << "        THEN: the input sequence has no effect\n";
    std::cout << "\n";
  }
#else
  if (word_buffer_.size() > 0)
  {
    word_buffer_.pop_back();
  }

  else if (line_buffer_.size() > 0)
  {
    word_buffer_ = line_buffer_.back();
    line_buffer_.pop_back();
  }
#endif
  break;

default:
#ifdef MEEVAX_DEBUG_KEYBIND
  std::cout << "[debug] FILE: " << __FILE__                       << std::endl;
  std::cout << "        LINE: " << __LINE__                       << std::endl;
  std::cout << "        ANSI: UNEXPECTED INPUT" << std::endl;
  std::cout << "        CODE: " << static_cast<int>(char_buffer_) << std::endl;
  std::cout << ""                                                 << std::endl;
#else
  std::cout << "";
#endif
  break;

