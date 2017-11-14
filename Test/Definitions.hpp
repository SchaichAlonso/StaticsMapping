#include <QtTest>

#include "DefinitionsHelper.hpp"

namespace Classification
{
  namespace Test
  {  
    struct Definitions : DefinitionsHelper
    {
      Q_OBJECT
      
    private slots:
      void removeAirlineFirst ();
      void removeAirlineMiddle ();
      void removeAirlineLast ();
      void removeAddAirline ();
    };
  }
}
