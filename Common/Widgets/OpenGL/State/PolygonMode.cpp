#include "PolygonMode.hpp"

namespace OpenGL {
  namespace State {
    PolygonMode::PolygonMode(GLenum face, GLenum mode)
    : m_front(current(GL_FRONT))
    , m_back(current(GL_BACK))
    {
      glPolygonMode(face, mode);
    }
    
    PolygonMode::~PolygonMode()
    {
      glPolygonMode(GL_FRONT, m_front);
      glPolygonMode(GL_BACK, m_back);
    }
    
    GLenum PolygonMode::current(GLenum face)
    {
      GLint buf[2];
      glGetIntegerv(GL_POLYGON_MODE, buf);
      return (face == GL_FRONT)? buf[0] : buf[1];
    }
  }
} 
