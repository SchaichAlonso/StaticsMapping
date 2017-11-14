#pragma once

#include <QtTest>

#include <Common/Classification/Model.hpp>
#include "DefinitionsHelper.hpp"

namespace Classification
{
  namespace Test
  {
    struct Models : DefinitionsHelper
    {
      Q_OBJECT
      
    private slots:
      void aircraft ();
      void airline ();
      void library ();
      void object ();
      void unlink ();
    };
  }
}
