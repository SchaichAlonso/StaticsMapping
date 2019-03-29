#include "Camera.hpp"

namespace OpenGL
{
  Camera::Camera(QString name)
  : Object()
  , m_zoom_stages{ 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610 }
  , m_zoom_active(m_zoom_stages.size() / 2)
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
    return (m_zoom_stages[m_zoom_active]);
  }
  
  
  void Camera::zoom(bool in)
  {
    if (in) {
      m_zoom_active = qMax(m_zoom_active-1, 0);
    } else {
      m_zoom_active = qMin(m_zoom_active+1, m_zoom_stages.size()-1);
    }
  }
  
  
  QMatrix4x4 Camera::projection() const
  {
    QMatrix4x4 projection;
#if 0
    double aspect((double)width()) / ((double)height());
    double d(zoom());
    
    if (m_ortho) {
      projection.ortho(
        -d/2, d/2,
        -d/(2*aspect), d/(2*aspect),
        -500, 500
      );
      modelview.rotate(m_pitch, 1, 0, 0);
      modelview.rotate(m_yaw, 0, 1, 0);
      
      observer = QVector3D(0, 0, 1);
    } else {
      projection.perspective(45, aspect, 0.01f, 1000);
      observer = d * sphericToCarthesian(m_pitch, m_yaw);
      
      modelview.lookAt(
        observer,
        QVector3D (0, 0, 0),
        QVector3D (0, 1, 0)
      );
    }
#endif
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
