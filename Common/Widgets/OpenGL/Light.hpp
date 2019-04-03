#pragma once

#include <QtCore/QSharedPointer>

#include <QtGui/QColor>
#include <QtGui/QVector3D>
#include <QtGui/QMatrix4x4>

#include "Object.hpp"

namespace OpenGL
{
  struct Light : Object
  {
    Light(QVector3D position, QColor color, QVector3D attenuation=QVector3D(1,0.01,0.0));
    
    QVector3D attenuation() const;
    QColor    color() const;
    
  protected:
    QColor m_color;
    QVector3D m_attenuation;
  };
  
  typedef QSharedPointer<Light> LightPointer;
} 
