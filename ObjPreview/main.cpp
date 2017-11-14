#include <QApplication>

#include "ObjPreview.hpp"

int
main (int argscnt, char **args)
{
  QApplication app (argscnt, args);
  app.setQuitOnLastWindowClosed (true);
  
  ObjPreview p;
  
  p.show ();
  
  if (argscnt >= 2) {
    p.loadObjFile (args[1]);
  }
  
  return (app.exec());
}
