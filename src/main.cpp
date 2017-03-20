#include <iostream>
#include <string>
#include <vector>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

int main(int argc, char** argv)
{
  QApplication app {argc, argv};

  QMainWindow window;
  window.show();

  return app.exec();
}
