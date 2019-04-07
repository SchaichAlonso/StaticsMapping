#include <QDateTime>
#include <QtMath>
#include "Light.hpp"

namespace OpenGL
{
  Attenuation::Attenuation(QVector3D attenuation)
  : gl_attenuation(attenuation)
  , range(-1)
  , range_exp(-1)
  {
  }
  
  Attenuation::Attenuation(float range, float exp)
  : gl_attenuation(1, 0, 0)
  , range(range)
  , range_exp(exp)
  {
  }
  
  SpotData::SpotData(QVector3D dir, float cutoff, float exp)
  : direction(dir)
  , cutoff_angle(cutoff)
  , exp(exp)
  {
  }
  
  SpotData::SpotData(QMatrix4x4 transformation, const SpotData &other)
  : direction(QVector3D(transformation * QVector4D(other.direction, 0)))
  , cutoff_angle(other.cutoff_angle)
  , exp(other.exp)
  {
  }
  
  Light::Light(QVector3D position, QColor color, Attenuation attenuation, SpotData spot)
  : Object(position)
  , m_color(color)
  , m_spot(spot)
  , m_attenuation(attenuation)
  {
  }
  
  
  Light::Light(QMatrix4x4 transformation, const OpenGL::Light& other)
  : Object(transformation * other.position())
  , m_color(other.color())
  , m_spot(transformation, other.m_spot)
  , m_attenuation(other.m_attenuation)
  {
  }

  
  
  QColor Light::color() const
  {
    return (m_color);
  }
  
  QVector3D Light::attenuation() const
  {
    return (m_attenuation.gl_attenuation);
  }
  
  float Light::range() const
  {
    return (m_attenuation.range);
  }
  
  float Light::rangeExp() const
  {
    return (m_attenuation.range_exp);
  }
  
  QVector3D Light::spotDirection() const
  {
    return (m_spot.direction);
  }
  
  float Light::spotCutoffAngle() const
  {
    return (m_spot.cutoff_angle);
  }
  
  float Light::spotExp() const
  {
    return (m_spot.exp);
  }
} 
