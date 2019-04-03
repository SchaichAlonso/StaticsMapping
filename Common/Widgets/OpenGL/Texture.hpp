#pragma once

#include <QtCore/QSharedPointer>
#include <QtGui/QImage>
#include <QtGui/QOpenGLTexture>

#include "RawImage.hpp"

namespace OpenGL
{
  struct Texture
  {
    typedef int Unit;
    
    Texture();
    Texture(RawImage image);
    Texture(QImage image);
   ~Texture();
    
    bool bind(Unit unit);
    void release(Unit unit);
    
  protected:
    static void release(QOpenGLTexture *instance);
    
  protected:
    QImage m_image;
    RawImage m_raw_data;
    QSharedPointer<QOpenGLTexture> m_texture;
    
  };
  
  typedef QSharedPointer<Texture> TexturePointer;
}
