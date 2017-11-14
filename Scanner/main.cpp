#include <QApplication>

#include "ScanDialog.hpp"

int
main (int argcnt, char **argsv)
{
  QApplication app (argcnt, argsv);
  
  ScanDialog scanner;
  scanner.exec();  
  
  return app.exec ();
}
