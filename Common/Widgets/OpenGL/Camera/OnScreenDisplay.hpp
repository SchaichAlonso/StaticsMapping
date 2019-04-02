#pragma once

#include "Camera.hpp"

namespace OpenGL
{
  struct OnScreenDisplay : Camera
  {
    OnScreenDisplay(QRect viewport);
    
    QMatrix4x4 projection() const Q_DECL_OVERRIDE;
    
  protected:
    QRect  m_rect;
  };
}
