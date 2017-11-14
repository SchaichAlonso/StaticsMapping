#pragma once

#include <QtTest>

namespace Classification
{
  namespace Test
  {
    struct XPClass : QObject
    {
      Q_OBJECT
      
      private slots:
        void vpaths ();
    };
  }
}
