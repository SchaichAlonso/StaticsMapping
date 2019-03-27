#include "Object.hpp"

namespace OpenGL
{
  Object::Object()
  : m_position(0, 0, 0)
  , m_roll(0)
  , m_pitch(0)
  , m_yaw(0)
  {
  }
  
  Object::~Object()
  {
  }
  
  void Object::move(QVector3D delta)
  {
    m_position = transform(transform(), delta);
  }
  
  QVector3D Object::position() const
  {
    return (m_position);
  }
  
  void Object::setPosition(QVector3D position)
  {
    m_position = position;
  }
  
  void Object::setOrientation(float roll, float pitch, float yaw)
  {
    m_roll = roll;
    m_pitch = pitch;
    m_yaw = yaw;
  }
  
  float Object::roll() const
  {
    return (m_roll);
  }
  
  void Object::setRoll(float roll)
  {
    setOrientation(roll, m_pitch, m_yaw);
  }
  
  float Object::pitch() const
  {
    return (m_pitch);
  }

  void Object::setPitch(float pitch)
  {
    setOrientation(m_roll, pitch, m_yaw);
  }
  
  float Object::yaw() const
  {
    return (m_yaw);
  }

  void Object::setYaw(float yaw)
  {
    setOrientation(m_roll, m_pitch, yaw);
  }
  
  QMatrix4x4 Object::transform() const
  {
    QMatrix4x4 roll, pitch, yaw, translate;
    
    roll.rotate(m_roll, QVector3D(0, 0, 1));
    pitch.rotate(m_pitch, QVector3D(1, 0, 0));
    yaw.rotate(m_yaw, QVector3D(0, 1, 0));
    translate.translate(m_position);
    
    QMatrix4x4 rotate(roll * pitch * yaw);
    
    return (translate * rotate);
  }
  
  QVector3D Object::transform(const QMatrix4x4 &transform, const QVector3D &point)
  {
    return (QVector3D(transform * QVector4D(point, 1)));
  }
}
