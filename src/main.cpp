#include <cstdlib>
#include <iostream>
#include <system_error>
#include <thread>

#include <meevax/vstream/basic_vstream.hpp>
#include <meevax/vstream/graphix_manipulator.hpp>
#include <meevax/vstream/graphix_operator.hpp>

#include <meevax/version.hpp>


int main(int argc, char** argv) try
{
  std::cout << "[debug] boost version: " << boost_version.data() << "\n";
  std::cout << "[debug] cairo version: " << cairo_version_string() << "\n\n";

  std::unique_ptr<Display, decltype(&XCloseDisplay)> display {XOpenDisplay(""), XCloseDisplay};

  meevax::visual_node<char> vstream {display.get()};

  vstream.raise() << "\e[1C;1C;1Cbg";

  vstream["title"].move((1280-640)/2, (720-200)/4).resize(640, 150).raise()
    << "\e[1C;1C;1Cbg";

  auto show_title = [&]()
  {
    vstream["title"]
      << "\e[1C;1C;1Cbg\e[D0;D0;D0fg"
      << meevax::face("Bitstream Charter")
      << meevax::size(90) << meevax::cursorhome
      << "Meevax System"
      << meevax::face("Sans")
      << meevax::size(35) << "\n" << meevax::size(25)
      << "Version " << project_version.data() << " Alpha "
      << meevax::size(16)
      << "(" << cmake_build_type.data() << " Build)";
  };

  vstream["debug"].move(600, (720-50)*3/4).resize(320, 50).raise()
    << "\e[1C;1C;1Cbg";

  vstream["escseq"].move(50, 400).resize(500, 300).raise()
    << "\e[1C;1C;1Cbg";

  auto show_hello_world = [&]()
  {
    vstream["escseq"]
      << "\e[1C;1C;1Cbg"
      << meevax::face("Ricty Diminished")
      << meevax::size(12.0 + 1.5 * 10)
      << meevax::cursorhome
      << "\e[AF;87;FFfg#include \e[FF;D7;5Ffg<iostream>\n\n"
      << "\e[87;D7;5Ffgint \e[D0;D0;D0fgmain("
      << "\e[87;D7;5Ffgint \e[D0;D0;D0fgargc, \e[87;D7;5Ffgchar\e[D0;D0;D0fg** argv)\n"
      << "{\n  std::cout << \e[FF;D7;5Ffg\"hello, world!\e[FF;5F;00fg"
      << "\e[FF;D7;5Ffg\"\e[D0;D0;D0fg;\n\n"
      << "  \e[FF;00;87fgreturn \e[00;D7;D7fg0\e[D0;D0;D0fg;\n}";
  };

  vstream["serial"].resize(300, 60).raise()
    << "\e[1C;1C;1Cbg";

  while (true)
  {
    auto event {vstream.event()};

    switch (event.type)
    {
    case Expose:
      vstream.resize(0, 0) << "\e[1C;1C;1Cbg";

      vstream["serial"]
        << meevax::face("Ricty Diminished") << meevax::size(12.0 + 1.5 * 10)
        << meevax::cursorhome
        << "\e[1C;1C;1Cbg\e[D0;D0;D0fgserial: " << std::to_string(event.xexpose.serial);

      show_title();
      show_hello_world();

      vstream["debug"]
        << meevax::face("Ricty Diminished") << meevax::size(12.0 + 1.5 * 10)
        << meevax::cursorhome
        << "\e[1C;1C;1Cbg\e[D0;D0;D0fg[debug] press any key";

      break;

    case KeyPress:
      vstream["debug"]
        << meevax::face("Ricty Diminished") << meevax::size(12.0 + 1.5 * 10)
        << meevax::cursorhome
        << "\e[1C;1C;1Cbg\e[D0;D0;D0fg[debug] " << XKeysymToString(XLookupKeysym(&event.xkey, 0));

      break;
    }
  }

  return 0;
}

catch (const std::logic_error& error)
{
  std::cerr << "[error] logic error occurred - " << error.what() << std::endl;
  std::exit(EXIT_FAILURE);
}

catch (const std::runtime_error& error)
{
  std::cerr << "[error] runtime error occurred - " << error.what() << std::endl;
  std::exit(EXIT_FAILURE);
}

catch (const std::system_error& error)
{
  std::cerr << "[error] system error occurred\n"
            << "        code: " << error.code().value() << " - " << error.code().message() << std::endl;
  std::exit(error.code().value());
}

catch (...)
{
  std::cerr << "[fatal] unexpected error occurred. report this to the developer.\n"
            << "        developer's email: httperror@404-notfound.jp\n";
  std::exit(errno);
}

