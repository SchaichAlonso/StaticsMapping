#include <Common/Classification/Aircraft.hpp>
#include <Common/Classification/Airline.hpp>
#include <Common/Classification/Library.hpp>

#include <Common/Widgets/OpenGL/Obj8Visitor.hpp>
#include "VisualObject.hpp"

namespace Widgets
{
  VisualObject::VisualObject(
    Classification::DefinitionsPointer definitions,
    OpenGL::ScenePointer scene,
    QString path
  )
  : file(new Obj8::File(path, true))
  , model(scene->insertModel())
  , data()
  {
    data = definitions->match(file->size(), file->fileHash(), file->textureHash());
    
    if (!data) {
      data.reset(new Classification::Object(file->size(), file->fileHash(), file->textureHash()));
      data->setFileName(file->basename());
    }
    
    QSharedPointer<OpenGL::Obj8Visitor> visitor(new OpenGL::Obj8Visitor(model, path));
    file->accept(visitor.data(), false);
  }
  
  
  
  VisualObject::~VisualObject ()
  {
  }
}
