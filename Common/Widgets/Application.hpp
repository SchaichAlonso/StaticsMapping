#pragma once

#include <QtWidgets/QApplication>

namespace Widgets
{
  struct ApplicationAttributes
  {
    ApplicationAttributes();
  };

  struct Application : ApplicationAttributes, QApplication
  {
    Application(int &argc, char **argv, int = ApplicationFlags);
    virtual ~Application();
  };
}
