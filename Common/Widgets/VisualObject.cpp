#include <Common/Classification/Aircraft.hpp>
#include <Common/Classification/Airline.hpp>
#include <Common/Classification/Library.hpp>

#include <Common/Widgets/OpenGL/Obj8Scene.hpp>
#include "VisualObject.hpp"

namespace Widgets
{
  VisualObject::VisualObject(
    Classification::DefinitionsPointer definitions,
    OpenGL::ScenePointer scene,
    QString path
  )
  : file(new Obj8::File(path, true))
  , model(qSharedPointerDynamicCast<OpenGL::Obj8Scene>(scene)->insertModel(file.get()))
  , data()
  {
    data = definitions->match(file->size(), file->fileHash(), file->textureHash());
    
    if (!data) {
      data.reset(new Classification::Object(file->size(), file->fileHash(), file->textureHash()));
      data->setFileName(file->basename());
    }
  }
  
  
  
  VisualObject::~VisualObject ()
  {
  }
}
