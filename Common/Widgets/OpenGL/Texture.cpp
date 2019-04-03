#include "Texture.hpp"

namespace OpenGL
{
  Texture::Texture()
  : m_image()
  , m_raw_data()
  , m_texture()
  {
  }
  
  
  Texture::Texture(RawImage data)
  : m_image()
  , m_raw_data(data)
  , m_texture()
  {
  }
  
  
  Texture::Texture(QImage img)
  : m_image(img)
  , m_raw_data()
  , m_texture()
  {
  }
  
  
  Texture::~Texture()
  {
  }
  
  
  bool
  Texture::bind(Unit unit)
  {
    if (m_texture.isNull()) {
      if ((m_image.width() > 0) && (m_image.height() > 0)) {
        m_texture.reset(new QOpenGLTexture(m_image));
        m_texture->setMinificationFilter(QOpenGLTexture::Linear);
      } else if(m_raw_data.isValid()) {
        m_texture.reset(new QOpenGLTexture(QOpenGLTexture::Target2D));
        m_texture->setFormat(QOpenGLTexture::RGBA32F);
        m_texture->setSize(m_raw_data.size().width(), m_raw_data.size().height());
        m_texture->setAutoMipMapGenerationEnabled(false);
        m_texture->allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::Float32);
        m_texture->setData(
          QOpenGLTexture::RGBA,
          QOpenGLTexture::Float32,
          m_raw_data.data().constData()
        );
        m_texture->setMinificationFilter(QOpenGLTexture::Linear);
      }
    }
    if (m_texture) {
      m_texture->bind(uint(unit), QOpenGLTexture::ResetTextureUnit);
    }
    return (m_texture && m_texture->isBound());
  }
  
  
  void
  Texture::release(Unit unit)
  {
    if (m_texture) {
      m_texture->release(uint(unit), QOpenGLTexture::ResetTextureUnit);
    }
  }
}

