#include "ZoomableCamera.hpp"

namespace OpenGL
{
  struct OrthoCamera : ZoomableCamera
  {
    OrthoCamera(QString name=QString(), QRect screen=QRect(), Zoom min=1, Zoom max=128, bool logarithmic=true);
    virtual ~OrthoCamera();
    
    QMatrix4x4 projection() const Q_DECL_OVERRIDE;
    bool wireframe() const Q_DECL_OVERRIDE;
  };
}
