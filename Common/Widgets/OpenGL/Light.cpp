#include "Light.hpp"

namespace OpenGL
{
  Light::Light(QVector3D position, QColor color, QVector3D attenuation)
  : Object(position)
  , m_color(color)
  , m_attenuation(attenuation)
  {
  }
  
  
  QVector3D
  Light::attenuation() const
  {
    return (m_attenuation);
  }
  
  
  QColor
  Light::color() const
  {
    return (m_color);
  }
} 
