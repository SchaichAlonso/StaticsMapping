#include "Camera.hpp"

namespace OpenGL
{
  Camera::Camera(QString name)
  : Object()
  , m_screen(-1, -1)
  , m_name(name)
  , m_fovy(30)
  , m_ortho(false)
  {
  }
  
  
  Camera::~Camera()
  {
  }
  
  QString Camera::name() const
  {
    return (m_name);
  }
  
  bool Camera::wireframe() const
  {
    return (false);
  }
  
  void Camera::move(QVector3D delta)
  {
    Object::move(-delta);
  }
  
  void Camera::setPosition(QVector3D position)
  {
    Object::setPosition(-position);
  }
  
  void Camera::setOrientation(float roll, float pitch, float yaw)
  {
    Object::setOrientation(-roll, -pitch, -yaw);
  }
  
  
  QSize Camera::screenDimensions() const
  {
    return (m_screen);
  }
  
  void Camera::setScreenDimensions(QSize size)
  {
    m_screen = size;
  }
  
  
  bool Camera::ortho() const
  {
    return (m_ortho);
  }
  
  
  void Camera::setOrtho(bool ortho)
  {
    m_ortho = ortho;
  }
  
  
  float Camera::fovy() const
  {
    return (m_fovy);
  }
  
  
  void Camera::setFovy(float fovy)
  {
    m_fovy = fovy;
  }
  
  
  float Camera::zoom() const
  {
    return (1.0);
  }
  
  
  void Camera::zoom(bool in)
  {
    Q_UNUSED(in);
  }
  
  
  QMatrix4x4 Camera::projection() const
  {
    QMatrix4x4 projection;
    float aspect(float(m_screen.width()) / m_screen.height());
    float distance(zoom());

    if (m_ortho) {
      float half(distance/2);
      projection.ortho(
        -half, half,
        -half/aspect, half/aspect,
        -500,  500
      );
    } else {
      projection.perspective(m_fovy, aspect, 0.01f, 1000);
    }
    
    return (projection);
  }
}
