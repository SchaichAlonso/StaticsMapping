#pragma once

#include <QtCore/QMultiMap>

#include "Classification.hpp"
#include "XPClass.hpp"

namespace Classification
{
  struct WeightedObject
  {
    typedef int Weight;
    
    WeightedObject (ObjectPointer =ObjectPointer(), Weight =0);
   ~WeightedObject ();
    
    bool operator== (const WeightedObject &other) const;
   
    ObjectPointer object;
    Weight weight;
  };
  
  typedef QMultiMap<XPClass, WeightedObject> WeightedObjectsByXPClass;
}
