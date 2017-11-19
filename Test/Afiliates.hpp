#include <QtTest>

#include <Classification/WeightedObject.hpp>

#include "DefinitionsHelper.hpp"

namespace Classification
{
  namespace Test
  {
    struct Afiliates : DefinitionsHelper
    {
      Q_OBJECT
    protected:
      static WeightedObject::Weight weight(QList<WeightedObject> list, ObjectPointer object);
    
    private slots:
      virtual void init () Q_DECL_OVERRIDE;
      
      void basic ();
      void sizing ();
      void explicitAge ();
      void implicitAge ();
    };
  }
}
