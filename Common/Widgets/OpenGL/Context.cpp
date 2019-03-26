#include <QtCore/QByteArray>
#include <QtCore/QMetaEnum>
#include <QtCore/QSet>
#include <QtGui/QImage>

#include "Context.hpp"

namespace OpenGL
{
  Context::Context(QSize window_size, int bpc)
  : m_surface(createSurface())
  , m_context(createContext())
  , m_fbo(createFBO(window_size, bpc))
  {
    m_context->makeCurrent(m_surface.data());
    QMap<QByteArray, int> extensions;
    Q_FOREACH(QByteArray ext, m_context->extensions().toList()) {
      extensions.insert(ext,1);
    }
    Q_FOREACH(QByteArray ext, extensions.keys()) {
      //qDebug("%s", ext.constData());
    }
    bool ok{m_fbo->bind()};
    Q_ASSERT(ok);
    m_context->functions()->glClearColor(0,1,1,1);
    m_context->functions()->glClear(GL_COLOR_BUFFER_BIT);
    m_fbo->release();
    
    QOpenGLFramebufferObject m_dpy(
      QSize(640,480),
      QOpenGLFramebufferObject::Depth,
      GL_TEXTURE_2D,
      GL_RGB8
    );
    
    QOpenGLFramebufferObject::blitFramebuffer(&m_dpy, QRect(0,0,640,480), m_fbo.data(), QRect(0,0,1024,1024), GL_COLOR_BUFFER_BIT, GL_NEAREST, 0, 0, QOpenGLFramebufferObject::DontRestoreFramebufferBinding);
    
    QImage fboImage{m_dpy.toImage()};
    /* QImage image{fboImage.constBits(), fboImage.width(), fboImage.height(), QImage::Format_ARGB32}; */
    fboImage.save("/tmp/image.png");
  }
  
  QOffscreenSurfacePointer Context::createSurface()
  {
    QOffscreenSurfacePointer retval(new QOffscreenSurface());
    retval->create();
    Q_ASSERT(retval->isValid());
    return (retval);
  }
  
  QOpenGLContextPointer Context::createContext()
  {
    QOpenGLContextPointer retval(new QOpenGLContext());
    retval->create();
    Q_ASSERT(retval->isValid());
    return (retval);
  }
  
  QOpenGLFramebufferObjectPointer Context::createFBO(QSize size, int bpc)
  {
    m_context->makeCurrent(m_surface.data());
    QOpenGLFramebufferObjectPointer retval(
      new QOpenGLFramebufferObject(
        size,
        QOpenGLFramebufferObject::Depth,
        GL_TEXTURE_2D,
        internalFormat(bpc)
      )
    );
    Q_ASSERT(retval->isValid());
    
    GLenum format{retval->format().internalTextureFormat()};
    qDebug("fbo format: %s\n", QMetaEnum::fromType<QOpenGLTexture::TextureFormat>().valueToKey(format));
    return (retval);
  }
  
  QOpenGLTexture::TextureFormat Context::internalFormat(int bpc)
  {
    switch (bpc) {
      case 8:
        return (QOpenGLTexture::RGBA8_UNorm);
      case 10:
        return (QOpenGLTexture::RGB10A2);
      case 16:
        return (QOpenGLTexture::RGBA16F);
      case 32:
        return (QOpenGLTexture::RGBA32F);
      default:
        qCritical("invalid bits-per-channel <%d>", bpc);
        return (QOpenGLTexture::RGBA8_UNorm);
    }
  }
  
  void Context::draw()
  {
    
  }
}
