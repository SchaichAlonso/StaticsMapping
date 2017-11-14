#pragma once

#include <QtCore/QObject>

#include <Common/Classification/Definitions.hpp>
#include <Common/Classification/Object.hpp>

namespace Classification
{
  namespace Test
  {
    struct DefinitionsHelper : QObject
    {
      Q_OBJECT
    
    public:
      static Object::Hash hash (int i);
      static Object::PrimaryKey objkey (int i);
    
      ObjectPointer object (int) const;
      
      void addAircraft (QString);
      void addAirline (QString);
      void addAirport (QString, double, double);
      void addLibrary (QString);
      void addObject (QString, QString, QString, int, bool);
  
    protected slots:
      virtual void init ();
      virtual void cleanup ();
    
    protected:
      DefinitionsPointer m_definitions;
    };
  }
}
