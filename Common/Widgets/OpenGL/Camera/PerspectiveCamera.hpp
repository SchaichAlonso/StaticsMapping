#include "ZoomableCamera.hpp"

namespace OpenGL
{
  struct PerspectiveCamera : ZoomableCamera
  {
    PerspectiveCamera(float fovy, QString name=QString(), QRect screen=QRect(), Zoom min=defaultMinimalZoom(), Zoom max=defaultMaximumZoom(), bool logarithmic=true);
    virtual ~PerspectiveCamera();
    
    QMatrix4x4 projection() const Q_DECL_OVERRIDE;
    
  protected:
    float m_fovy;
  };
}
