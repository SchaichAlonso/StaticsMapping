#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QSize>
#include <QtGui/QOffscreenSurface>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFramebufferObject>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLTexture>

typedef QSharedPointer<QOffscreenSurface> QOffscreenSurfacePointer;
typedef QSharedPointer<QOpenGLContext> QOpenGLContextPointer;
typedef QSharedPointer<QOpenGLFramebufferObject> QOpenGLFramebufferObjectPointer;

namespace OpenGL
{
  struct Context
  {
  public:
    Context(QSize window_size, int bpc);
    
    void draw();
    
  protected:
    QOffscreenSurfacePointer createSurface();
    QOpenGLContextPointer createContext();
    QOpenGLFramebufferObjectPointer createFBO(QSize size, int bpc);
    QOpenGLTexture::TextureFormat internalFormat(int bpc);
    
    
  protected:
    QOffscreenSurfacePointer m_surface;
    QOpenGLContextPointer m_context;
    QOpenGLFramebufferObjectPointer m_fbo;
  };
}
