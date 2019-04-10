#include <OpenGL/API.hpp>

#include "DepthMask.hpp"

namespace OpenGL
{
  namespace State
  {
    DepthMask::DepthMask(GLboolean write_enable)
    : m_previous(currentSetting())
    {
      OpenGL::api()->glDepthMask(write_enable);
    }
      
    DepthMask::~DepthMask()
    {
      OpenGL::api()->glDepthMask(m_previous);
    }
      
    GLboolean DepthMask::currentSetting()
    {
      GLboolean value;
      OpenGL::api()->glGetBooleanv(GL_DEPTH_WRITEMASK, &value);
      return (value);
    }
  }
}

