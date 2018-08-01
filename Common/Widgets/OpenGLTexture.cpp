#include "OpenGLTexture.hpp"

OpenGLTexture::OpenGLTexture(QOpenGLWidget *widget, QImage image)
  : m_widget(widget)
  , m_image(image)
  , m_texture()
{
}


void
OpenGLTexture::bind()
{
  if (!m_image.isNull()) {
    if (m_widget->isValid()) {
      if (m_texture.isNull()) {
        m_texture = construct(m_image);
        m_texture->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
      }
      m_texture->bind();
    }
  }
}



QSharedPointer<QOpenGLTexture>
OpenGLTexture::construct(QImage image)
{
  return QSharedPointer<QOpenGLTexture>(
    new QOpenGLTexture(image),
    [this](QOpenGLTexture *t) {
       destroy(t);
    }
  );
}



void OpenGLTexture::destroy(QOpenGLTexture *t)
{
  Q_CHECK_PTR(t);
  
  m_widget->makeCurrent();
  delete(t);
  m_widget->doneCurrent();
}
