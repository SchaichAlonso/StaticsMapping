#pragma once

#include <QtCore/QSharedPointer>

#include <QtGui/QColor>
#include <QtGui/QVector3D>
#include <QtGui/QMatrix4x4>

#include "Object.hpp"

namespace OpenGL
{
  struct SpotData
  {
    SpotData(QVector3D dir=QVector3D(), float cutoff=360.0, float exp=1);
    SpotData(QMatrix4x4 transformation, const SpotData &other);
    
    QVector3D direction;
    float cutoff_angle;
    float exp;
  };
  
  struct Attenuation
  {
    Attenuation(QVector3D attenuation);
    Attenuation(float range, float exp=2);
    
    QVector3D gl_attenuation;
    float range;
    float range_exp;
  };
  
  struct Light : Object
  {
    Light(QVector3D position, QColor color, Attenuation attenuation=Attenuation(50.0), SpotData spot=SpotData());
    Light(QMatrix4x4 transformation, const Light &other);
    
    QColor    color() const;
    
    QVector3D attenuation() const;
    float     range() const;
    float     rangeExp() const;
    
    QVector3D spotDirection() const;
    float     spotCutoffAngle() const;
    float     spotExp() const;
    
  protected:
    QColor m_color;
    
    SpotData m_spot;
    Attenuation m_attenuation;
  };
  
  typedef QSharedPointer<Light> LightPointer;
} 
