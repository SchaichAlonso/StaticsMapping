#pragma once

#include <QtCore/QFile>
#include <QtCore/QSharedPointer>

#include <Common/Obj8/File.hpp>
#include <Common/Classification/Definitions.hpp>
#include <Common/Classification/Object.hpp>

#include "VisualModel.hpp"

struct VisualObject
{
  VisualObject (Classification::DefinitionsPointer, QFile &);
  ~VisualObject ();
  
  QSharedPointer<Obj8::File>   file;
  QSharedPointer<VisualModel>  model;
  Classification::ObjectPointer data;
};

typedef QSharedPointer<VisualObject> VisualObjectPointer;
