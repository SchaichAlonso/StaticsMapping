#pragma once

#include <QtCore/QSharedPointer>
#include <QtGui/QImage>
#include <QtGui/QOpenGLTexture>
#include <QtWidgets/QOpenGLWidget>

struct OpenGLTexture
{
  OpenGLTexture(QOpenGLWidget *widget, QImage image);
  
  void bind();
  
protected:
  QOpenGLWidget *m_widget;
  QImage m_image;
  QSharedPointer<QOpenGLTexture> m_texture;
};

typedef QSharedPointer<OpenGLTexture> OpenGLTexturePointer;
