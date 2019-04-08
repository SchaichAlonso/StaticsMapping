#pragma once

#include <QtCore/QMap>
#include <QtGui/QVector3D>

#include "Light.hpp"

namespace OpenGL
{
  struct Obj8NamedLightFactory
  {
    Obj8NamedLightFactory();
    
    LightPointer create(QVector3D position, QString name);
  
  protected:
    typedef QMap<QString,LightPointer> KnownLights;
    
    static KnownLights init();
    
    KnownLights m_lights;
  };
}
