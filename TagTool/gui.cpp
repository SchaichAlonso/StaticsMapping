#include <QApplication>
#include <QFile>

#include "MainWindow.hpp"


int
main (int argscnt, char **args)
{
  QApplication app (argscnt, args);
  app.setQuitOnLastWindowClosed (true);
  
  MainWindow w;
  w.show ();
  for (int i=1; i<argscnt; ++i) {
    w.loadObjFile (args[i]);
  }
  return (app.exec());
}
