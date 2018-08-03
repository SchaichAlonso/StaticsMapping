#include <Common/Widgets/Application.hpp>

#include "ObjPreview.hpp"

int
main (int argscnt, char **args)
{
  Widgets::Application app(argscnt, args);
  
  ObjPreview p;
  
  p.show ();
  
  if (argscnt >= 2) {
    p.loadObjFile (args[1]);
  }
  
  return (app.exec());
}
