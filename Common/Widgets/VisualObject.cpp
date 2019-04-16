#include <Common/Classification/Aircraft.hpp>
#include <Common/Classification/Airline.hpp>
#include <Common/Classification/Library.hpp>

#include <Common/Widgets/OpenGL/Obj8Scene.hpp>
#include "VisualObject.hpp"

namespace Widgets
{
  VisualObject::VisualObject(Classification::ObjectPointer data, QSharedPointer<Obj8::File> file, OpenGL::ModelWeakPointer model)
  : file(file)
  , model(model)
  , data(data)
  {
  }
  
  VisualObject::VisualObject(Classification::DefinitionsPointer definitions, QSharedPointer<Obj8::File> file, OpenGL::ScenePointer scene)
  : VisualObject(
    definitions->match(file->size(), file->fileHash(), file->textureHash()),
    file,
    qSharedPointerDynamicCast<OpenGL::Obj8Scene>(scene)->insertModel(file.get())
  )
  {
  }
  
  VisualObject::VisualObject(
    Classification::DefinitionsPointer definitions,
    OpenGL::ScenePointer scene,
    QString path
  )
  : VisualObject(
      definitions,
      QSharedPointer<Obj8::File>(new Obj8::File(path, true)),
      scene
    )
  {
    if (!data) {
      data.reset(new Classification::Object(file->size(), file->fileHash(), file->textureHash()));
      data->setFileName(file->basename());
    }
  }
  
  
  
  VisualObject::~VisualObject ()
  {
  }
}
