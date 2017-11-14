#include <QtTest>

#include "DefinitionsHelper.hpp"

namespace Classification
{
  namespace Test
  {
    struct Afiliates : DefinitionsHelper
    {
      Q_OBJECT
    private slots:
      virtual void init () Q_DECL_OVERRIDE;
      
      void basic ();
      void sizing ();
      void explicitAge ();
      void implicitAge ();
    };
  }
}
