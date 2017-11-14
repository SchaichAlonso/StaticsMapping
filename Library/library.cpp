#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QIODevice>

#include <Classification/Afiliates.hpp>
#include <Classification/Airline.hpp>
#include <Classification/Definitions.hpp>
#include <Classification/Object.hpp>


using namespace Classification;




int
main (int args, char **argsv)
{
  QCoreApplication app (args, argsv);
  
  Q_ASSERT (args == 2);
  
  QDir dir(argsv[1]);
  dir.cdUp();
  QString outpath = dir.absoluteFilePath("library.txt");
  
  QFile output(outpath);
  if (not output.open (QFile::WriteOnly | QFile::Truncate)) {
    qFatal ("Failed to write %s", qUtf8Printable(outpath));
  }
  
  DefinitionsPointer defs (Definitions::fromFile (argsv[1]));
  Afiliations a (defs);
  XPLibrary lib = a.library ();
  
  output.write (lib.toString(true).toUtf8());
  
  return (EXIT_SUCCESS);
}
