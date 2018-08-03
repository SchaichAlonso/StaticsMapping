#include "Application.hpp"

namespace Widgets
{
  Application::Application(int &argc, char **argv, int flags)
    : QApplication (argc, argv, flags)
  {
    setAttribute(Qt::AA_UseDesktopOpenGL);
    setQuitOnLastWindowClosed(true);
  }
  
  Application::~Application()
  {
  }
}
