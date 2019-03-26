#pragma once

#include <QtCore/QSharedPointer>

namespace OpenGL
{
  struct Obj8Lod
  {
    Obj8Lod(int lod_near, int lod_far);
    
    bool enabled();
    
    int lod_near;
    int lod_far;
  };
  
  QSharedPointer<Obj8Lod> Obj8LodPointer;
} 
