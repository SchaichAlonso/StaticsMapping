#include <QApplication>

#include "ScanDialog.hpp"

int
main (int argcnt, char **argsv)
{
  QApplication app (argcnt, argsv);
  app.setQuitOnLastWindowClosed (true);
  
  ScanDialog scanner;
  scanner.exec();  
  
  return app.exec ();
}
