#pragma once

#include <QtCore/QString>

#include "Classification.hpp"
#include "WeightedObject.hpp"
#include "XPClass.hpp"


namespace Classification
{
  struct XPLibrary
  {
    XPLibrary (DefinitionsPointer);
   ~XPLibrary ();
    
    void add (XPClass, ObjectPointer, QString =QString());
    void add (WeightedObjectsByXPClass, QString =QString());
    
    QString toString (bool) const;
    
  protected:
    QMultiMap<QString, QString> m_map;
    
    DefinitionsPointer m_definitions;
  };
}
