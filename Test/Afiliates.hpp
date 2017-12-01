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
      
      void regional ();
      void relatives ();
      void resizing ();
      void explicitAge ();
      void implicitAge ();
      
    private:
      AirlinePointer FFM, MAS;
      AirlinePointer AFL, PLK, SDM;
      
      AircraftPointer E135, E75L, SU95, AT75;
    };
  }
}
