#pragma once

#include <QtCore/QSize>
#include <QtCore/QSharedPointer>
#include <QtGui/QOpenGLFramebufferObject>
#include <QtGui/QOpenGLTexture>

namespace OpenGL {
  
  struct FrameBufferObject
  {
    typedef QOpenGLTexture::TextureFormat Format;
    typedef QSharedPointer<QOpenGLFramebufferObject> Handle;
  
    FrameBufferObject(Format format=QOpenGLTexture::RGBA32F);
    
    QSize size() const;
    
    bool bind(QSize viewport);
    bool release();
    void blit(QRect target);
    
    Handle handle();
    
  protected:
    Handle m_handle;
    QSize m_size;
    Format m_format;
  };
}
