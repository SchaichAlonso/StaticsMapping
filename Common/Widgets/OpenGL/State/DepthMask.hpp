#pragma once

#include <QtGui/QOpenGLContext>

namespace OpenGL
{
  namespace State
  {
    struct DepthMask
    {
      DepthMask(GLboolean write_enable);    
      ~DepthMask();
    
      static GLboolean currentSetting();
    
    protected:
      GLboolean m_previous;
    };
  }
}
