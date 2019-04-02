#include "OrthoCamera.hpp"

namespace OpenGL
{
  OrthoCamera::OrthoCamera(QString name, QRect screen, Zoom min, Zoom max, bool logarithmic)
  : ZoomableCamera(screen, name, min, max, logarithmic)
  {
  }
  
  OrthoCamera::~OrthoCamera()
  {
  }
  
  QMatrix4x4 OrthoCamera::projection() const
  {
    QMatrix4x4 projection;
    float aspect(float(m_screen.width()) / m_screen.height());
    float half(zoom()/2);
    
    projection.ortho(
      -half, half,
      -half/aspect, half/aspect,
      -500,  500
    );
    
    return (projection);
  }
  
  bool OrthoCamera::wireframe() const
  {
    return (true);
  }
}
