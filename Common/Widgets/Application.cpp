#include "Application.hpp"

namespace Widgets
{
  ApplicationAttributes::ApplicationAttributes()
  {
    QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    QApplication::setQuitOnLastWindowClosed(true);
  }

  Application::Application(int &argc, char **argv, int flags)
    : ApplicationAttributes()
    , QApplication (argc, argv, flags)
  {
  }
  
  Application::~Application()
  {
  }
}
