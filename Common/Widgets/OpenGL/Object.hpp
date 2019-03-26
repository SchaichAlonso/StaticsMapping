#pragma once

#include <QtGui/QVector3D>
#include <QtGui/QMatrix4x4>

namespace OpenGL
{
  struct Object
  {
    Object();
    virtual ~Object();
    
    virtual void move(QVector3D delta);
    virtual void setPosition(QVector3D position);
    virtual void setOrientation(float roll, float pitch, float yaw);
    
    void setRoll(float roll);
    void setPitch(float pitch);
    void setYaw(float yaw);
    
    static QVector3D transform(const QMatrix4x4 &transform, const QVector3D &point);
    QMatrix4x4 transform() const;
    
    
  protected:
    QVector3D m_position;
    
    float m_roll;
    float m_pitch;
    float m_yaw;
  };
}
