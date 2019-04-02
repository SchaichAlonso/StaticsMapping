#include "OnScreenDisplay.hpp"

namespace OpenGL
{
  OnScreenDisplay::OnScreenDisplay(QRect viewport)
  : Camera()
  , m_rect(viewport)
  {
  }
  
  QMatrix4x4 OnScreenDisplay::projection() const
  {
    QMatrix4x4 projection;
    projection.ortho(m_rect);
    return (projection);
  }
}
