#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <Common/Obj8/File.hpp>
#include <Common/Classification/Definitions.hpp>
#include <Common/Classification/Object.hpp>

#include "VisualModel.hpp"

namespace Widgets
{
  struct VisualObject
  {
    VisualObject(Classification::DefinitionsPointer, QString);
    ~VisualObject();
  
    QSharedPointer<Obj8::File>   file;
    QSharedPointer<VisualModel>  model;
    Classification::ObjectPointer data;
  };

  typedef QSharedPointer<VisualObject> VisualObjectPointer;
}
