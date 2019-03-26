#include "Texture.hpp"

namespace OpenGL
{
  Texture::Texture(QImage img)
  : m_image(img)
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

