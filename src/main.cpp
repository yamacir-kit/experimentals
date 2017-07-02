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
  XSynchronize(display.get(), true);

  meevax::visual_node<char> vstream {display.get()};

  vstream.raise() << "\e[28;28;28M";

  vstream["title"].move((1280-640)/2, (720-200)/4).resize(640, 150).raise()
    << "\e[28;28;28M";

  auto show_title = [&]()
  {
    vstream["title"]
      << "\e[28;28;28M\e[208;208;208m"
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
    << "\e[28;28;28M";

  vstream["escseq"].move(50, 400).resize(500, 300).raise()
    << "\e[28;28;28M";

  auto show_hello_world = [&]()
  {
    vstream["escseq"]
      << "\e[28;28;28M"
      << meevax::face("Ricty Diminished")
      << meevax::size(12.0 + 1.5 * 10)
      << meevax::cursorhome
      << "\e[175;135;255m#include \e[255;215;95m<iostream>\n\n"
      << "\e[135;215;95mint \e[208;208;208mmain("
      << "\e[135;215;95mint \e[208;208;208margc, \e[135;215;95mchar\e[208;208;208m** argv)\n"
      << "{\n  std::cout << \e[255;215;95m\"hello, world!\e[255;95;00m"
      << "\e[255;215;95m\"\e[208;208;208m;\n\n"
      << "  \e[255;00;135mreturn \e[00;215;215m0\e[208;208;208m;\n}";
  };

  vstream["serial"].resize(300, 60).raise()
    << "\e[28;28;28M";

  while (true)
  {
    auto event {vstream.event()};

    switch (event.type)
    {
    case Expose:
      if (!event.xexpose.count)
      {
        vstream.resize(0, 0) << "\e[28;28;28M";

        vstream["serial"]
          << meevax::face("Ricty Diminished") << meevax::size(12.0 + 1.5 * 10)
          << meevax::cursorhome
          << "\e[28;28;28M\e[208;208;208mserial: " << std::to_string(event.xexpose.serial);

        show_title();
        show_hello_world();

        vstream["debug"]
          << meevax::face("Ricty Diminished") << meevax::size(12.0 + 1.5 * 10)
          << meevax::cursorhome
          << "\e[28;28;28M\e[208;208;208m[debug] press any key";
      }

      break;

    case KeyPress:
      vstream["debug"]
        << meevax::face("Ricty Diminished") << meevax::size(12.0 + 1.5 * 10)
        << meevax::cursorhome
        << "\e[28;28;28M\e[208;208;208m[debug] " << XKeysymToString(XLookupKeysym(&event.xkey, 0));

      break;
    }
  }

  return 0;
}

catch (const std::exception& error)
{
  std::cerr << "[error] standard exception occurred - " << error.what() << std::endl;
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

