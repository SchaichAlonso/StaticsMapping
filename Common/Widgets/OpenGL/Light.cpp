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
  
  Attenuation::Attenuation(QJsonObject json)
  : gl_attenuation(
      json["constant"].toDouble(),
      json["linear"].toDouble(),
      json["quadratic"].toDouble()
    )
  , range(json["range"].toDouble())
  , range_exp(json["rangeExp"].toDouble())
  {
  }
  
  QJsonObject Attenuation::toJson() const
  {
    QJsonObject retval;
    retval.insert("constant", gl_attenuation.x());
    retval.insert("linear", gl_attenuation.y());
    retval.insert("quadratic", gl_attenuation.z());
    retval.insert("range", range);
    retval.insert("rangeExp", range_exp);
    return (retval);
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
  
  SpotData::SpotData(QJsonObject json)
  : direction(
      json["x"].toDouble(),
      json["y"].toDouble(),
      json["z"].toDouble()
    )
  , cutoff_angle(json["cutoff"].toDouble())
  , exp(json["exp"].toDouble())
  {
  }
  
  QJsonObject SpotData::toJson() const
  {
    QJsonObject retval;
    retval.insert("cutoff", cutoff_angle);
    retval.insert("exp", exp);
    retval.insert("x", direction.x());
    retval.insert("y", direction.y());
    retval.insert("z", direction.z());
    return (retval);
  }
  
  
  Light::Light(QJsonObject json)
  : Light(
      QColor::fromRgb(
        json["r"].toInt(),
        json["g"].toInt(),
        json["b"].toInt()
      ),
      Attenuation(json["attenuation"].toObject()),
      SpotData(json["spot"].toObject())
    )
  {
  }
  
  QJsonObject Light::toJson() const
  {
    QJsonObject retval;
    retval.insert("r", m_color.red());
    retval.insert("g", m_color.green());
    retval.insert("b", m_color.blue());
    retval.insert("attenuation", m_attenuation.toJson());
    retval.insert("spot", m_spot.toJson());
    return (retval);
  }
  
  Light::Light(QColor color, Attenuation attenuation, SpotData spot)
  : Light(QVector3D(), color, attenuation, spot)
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
