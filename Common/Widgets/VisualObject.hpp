#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <Common/Obj8/File.hpp>
#include <Common/Classification/Definitions.hpp>
#include <Common/Classification/Object.hpp>

#include <Common/Widgets/OpenGL/Model.hpp>
#include <Common/Widgets/OpenGL/Scene.hpp>

#include "VisualModel.hpp"

namespace Widgets
{
  struct VisualObject
  {
    VisualObject(Classification::ObjectPointer data, QSharedPointer<Obj8::File> file, OpenGL::ModelWeakPointer model);
    VisualObject(Classification::DefinitionsPointer, QSharedPointer<Obj8::File> file, OpenGL::ScenePointer scene);
    VisualObject(Classification::DefinitionsPointer, OpenGL::ScenePointer, QString filename);
    ~VisualObject();
  
    QSharedPointer<Obj8::File> file;
    OpenGL::ModelWeakPointer model;
    Classification::ObjectPointer data;
  };

  typedef QSharedPointer<VisualObject> VisualObjectPointer;
}
