#include "PerspectiveCamera.hpp"

namespace OpenGL
{
  PerspectiveCamera::PerspectiveCamera(float fovy, QString name, QRect screen, Zoom min, Zoom max, bool logarithmic)
  : ZoomableCamera(screen, name, min, max, logarithmic)
  , m_fovy(fovy)
  {
  }
  
  PerspectiveCamera::~PerspectiveCamera()
  {
  }
  
  QMatrix4x4 PerspectiveCamera::projection() const
  {
    QMatrix4x4 retval;
    
    float aspect(float(m_screen.width()) / m_screen.height());
    
    retval.perspective(m_fovy, aspect, 0.1, 1000);
    
    return (retval);
  }
}
