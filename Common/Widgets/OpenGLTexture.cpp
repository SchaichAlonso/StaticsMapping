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
        m_texture.reset(new QOpenGLTexture(m_image));
        m_texture->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
      }
      m_texture->bind();
    }
  }
}
