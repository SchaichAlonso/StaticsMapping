#pragma once

#include <QtWidgets/QApplication>

namespace Widgets
{
  struct Application : QApplication
  {
    Application(int &argc, char **argv, int = ApplicationFlags);
    virtual ~Application();
  };
}
