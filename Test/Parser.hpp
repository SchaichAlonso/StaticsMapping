#include <QtTest>

#include <Common/Obj8/Parser/Number.hpp>

#include "ParserHelper.hpp"

namespace Obj8
{
  namespace Test
  {
    struct Parser : QObject, ParserHelper
    {
      Q_OBJECT
    
    protected:
      void addsub (QString, QString, QString, bool);
      
    private slots:
      void generic ();
      void math ();
      void lexerContext ();
      void reprocudesNumbers ();
      void poorlyFormatedNumbers ();
      void reproducesWords ();
      void whitespace ();
      void endOfLine ();
    };
  }
}
