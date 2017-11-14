#include <Common/Classification/Aircraft.hpp>
#include <Common/Classification/Airline.hpp>
#include <Common/Classification/Library.hpp>

#include "VisualObject.hpp"


VisualObject::VisualObject (Classification::DefinitionsPointer definitions, QFile &f)
  : file (new Obj8::File (f, true))
  , model (new VisualModel (f.fileName ()))
  , data ()
{
  data = definitions->match (file->size(), file->fileHash(), file->textureHash());
  
  if (!data) {
    data.reset (new Classification::Object(file->size(), file->fileHash(), file->textureHash()));
    data->setFileName (file->basename());
  }
  
  file->accept (model.data(), false);
}



VisualObject::~VisualObject ()
{
}
