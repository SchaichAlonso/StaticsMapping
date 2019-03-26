#pragma once

#include <QtCore/QSharedPointer>

#include <QtGui/QVector3D>
#include <QtGui/QMatrix4x4>

namespace OpenGL
{
  struct Light
  {
    Light(QVector3D position, QVector3D color, QVector3D attenuation=QVector3D(1,0.01,0.0));
    
    QVector3D attenuation() const;
    QVector3D position() const;
    QVector3D color() const;
    
  protected:
    QVector3D m_position;
    QVector3D m_color;
    QVector3D m_attenuation;
  };
  
  typedef QSharedPointer<Light> LightPointer;
} 
