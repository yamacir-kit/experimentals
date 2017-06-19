#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <system_error>
#include <thread>

#include <meevax/core/generic_editor.hpp>

#include <meevax/vstream/basic_vstream.hpp>
#include <meevax/vstream/vstream_manipulator.hpp>
#include <meevax/vstream/vstream_operator.hpp>


int main(int argc, char** argv)
{
  if (false)
  {
    meevax::generic_editor<char> editor {argc, argv};

    try
    {
      while (true)
      {
        editor.write();
        editor.read();
      }
    }

    catch (std::system_error& error)
    {
      std::cerr << "[error] code: " << error.code().value() << " - " << error.code().message() << std::endl;
      return error.code().value();
    }

    catch (...)
    {
      std::cerr << "[fatal] An unexpected error occurred. Report the following output to the developer.\n"
                << "\n"
                << "\tdeveloper's email: httperror@404-notfound.jp\n"
                << "\terrno: " << errno << " - " << std::strerror(errno) << std::endl;

      std::exit(errno);
    }
  }


  meevax::basic_vstream<char> vstream {""};

  vstream["master"] << meevax::raise
                    << meevax::face("Ricty Diminished")
                    << meevax::size(20)
                    << meevax::color(0, 0, 0)
                    << meevax::cursorhome
                    << "hogehoge\nfugafuga" << meevax::endl
                    << "piyipiyo"           << meevax::endl;

  while (true)
  {
    auto event {vstream.next_event()};
    switch (event.type)
    {
    case Expose:
      std::cout << "[debug] expose\n";
      break;

    case KeyPress:
      vstream["master"] << meevax::color(1, 1, 1) << meevax::paint
                        << meevax::color(0, 0, 0) << meevax::cursorhome
                        << "[debug] " << std::string(XKeysymToString(XLookupKeysym(&event.xkey, 0))).c_str() << meevax::endl;
      break;
    }
  }


  return 0;
}
