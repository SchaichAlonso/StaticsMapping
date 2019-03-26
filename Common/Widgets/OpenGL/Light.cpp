#include "Light.hpp"

namespace OpenGL
{
  Light::Light(QVector3D position, QVector3D color, QVector3D attenuation)
  : m_position(position)
  , m_color(color)
  , m_attenuation(attenuation)
  {
  }
  
  
  QVector3D
  Light::attenuation() const
  {
    return (m_attenuation);
  }
  
  
  QVector3D
  Light::color() const
  {
    return (m_color);
  }
  
  
  QVector3D
  Light::position() const
  {
    return (m_position);
  }
} 
