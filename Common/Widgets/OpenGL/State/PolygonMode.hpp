#include <QtGui/QOpenGLContext>

namespace OpenGL {
  namespace State {
    struct PolygonMode
    {
      PolygonMode(GLenum face, GLenum mode);
     ~PolygonMode();
      
      static GLenum current(GLenum face);
      
    protected:
      GLenum m_front;
      GLenum m_back;
    };
  }
}
