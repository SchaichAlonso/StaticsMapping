#include "FrameBufferObject.hpp"

namespace OpenGL 
{
  FrameBufferObject::FrameBufferObject(Format format)
  : m_handle()
  , m_size()
  , m_format(format)
  {
  }
  
  QSize FrameBufferObject::size() const
  {
    return (m_size);
  }
  
  bool FrameBufferObject::bind(QSize viewport)
  {
    if (m_handle) {
      if (m_handle->size() != viewport) {
        m_handle.reset();
      }
    }
    
    m_size = viewport;
    
    if (m_handle.isNull()) {
      m_handle.reset(
        new QOpenGLFramebufferObject(
          m_size,
          QOpenGLFramebufferObject::Depth,
          GL_TEXTURE_2D,
          m_format
        )
      );
      Q_ASSERT (m_handle->isValid());
    }
    
    return m_handle->bind();
  }
  
  bool FrameBufferObject::release()
  {
    if (m_handle) {
      return m_handle->release();
    }
    return true;
  }
  
  FrameBufferObject::Handle FrameBufferObject::handle()
  {
    return m_handle;
  }
  
  void FrameBufferObject::blit(QOpenGLFramebufferObject *target, QRect size)
  {
    if (m_handle && m_handle->isValid()) {
      QRect src(QPoint(0, 0), m_size);
      
      QOpenGLFramebufferObject::blitFramebuffer(
        target,
        size,
        m_handle.get(),
        src,
        GL_COLOR_BUFFER_BIT,
        GL_LINEAR
      );
    }
  }
}
