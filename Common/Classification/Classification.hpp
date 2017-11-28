#pragma once

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>

#include <Common/Common.hpp>

namespace Classification
{
#define DECLARE_STRUCT(x) \
  struct x; \
  typedef QSharedPointer<x> x##Pointer
    
  DECLARE_STRUCT (Afiliations);
  DECLARE_STRUCT (Aircraft);
  DECLARE_STRUCT (Airline);
  DECLARE_STRUCT (Airport);
  DECLARE_STRUCT (Definitions);
  DECLARE_STRUCT (Library);
  DECLARE_STRUCT (Object);
  DECLARE_STRUCT (Record);
  DECLARE_STRUCT (XPClass);
  
  DECLARE_STRUCT (AirlineModel);
  DECLARE_STRUCT (AircraftModel);
  DECLARE_STRUCT (LibraryModel);
  DECLARE_STRUCT (Model);
  DECLARE_STRUCT (ObjectModel);
  
  typedef QSharedPointer<QObject> QObjectPointer;
  
#undef DECLARE_STRUCT
}
