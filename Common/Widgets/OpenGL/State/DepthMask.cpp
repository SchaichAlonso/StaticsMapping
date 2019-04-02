#include "DepthMask.hpp"

namespace OpenGL
{
  namespace State
  {
    DepthMask::DepthMask(GLboolean write_enable)
    : m_previous(currentSetting())
    {
      glDepthMask(write_enable);
    }
      
    DepthMask::~DepthMask()
    {
      glDepthMask(m_previous);
    }
      
    GLboolean DepthMask::currentSetting()
    {
      GLboolean value;
      glGetBooleanv(GL_DEPTH_WRITEMASK, &value);
      return (value);
    }
  }
}

