#pragma once

#include <QtCore/QSharedPointer>
#include <QtGui/QImage>
#include <QtGui/QOpenGLTexture>

namespace OpenGL
{
  struct Texture
  {
    typedef int Unit;
    
    Texture(QImage image=QImage());
   ~Texture();
    
    bool bind(Unit unit);
    void release(Unit unit);
    
  protected:
    static void release(QOpenGLTexture *instance);
    
  protected:
    QImage m_image;
    QSharedPointer<QOpenGLTexture> m_texture;
    
  };
  
  typedef QSharedPointer<Texture> TexturePointer;
}
