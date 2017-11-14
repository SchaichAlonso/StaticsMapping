#include <iostream>

#include <QCoreApplication>

#include <Classification/Definitions.hpp>
#include <Classification/Object.hpp>
#include <Obj8/File.hpp>

int
main (int argscnt, char **args)
{
  QCoreApplication app (argscnt, args);
  
  Classification::DefinitionsPointer definitions(
      Classification::Definitions::fromFile ()
  );
  
  for (int i=1; i<argscnt; ++i) {
    QFile qf (args[i]);
    if (not qf.open (QFile::ReadOnly)) {
      qCritical ("Failed to open %s", args[i]);
      continue;
    }
    
    if (definitions->couldMatch (qf.size())) {
      Obj8::File file (qf, false);
      if (definitions->couldMatch (file.fileHash())) {
        try {
          file.parse ();
          Classification::ObjectPointer obj;
          obj = definitions->match (file.size(), file.fileHash(), file.textureHash());
          
          if (obj) {
            std::cout << args[i] << std::endl;
          }
          
        } catch (...) {
          qCritical ("size and primary hash match for %s.", args[i]);
        }
      }
    }
  }
  
  Q_UNUSED (app);
  return (EXIT_SUCCESS);
}
