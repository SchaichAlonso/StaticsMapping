#pragma once

#include <QtCore/QList>

#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QVector3D>

#include <QtWidgets/QOpenGLWidget>

#include "OpenGLTexture.hpp"

struct OpenGLWidget : QOpenGLWidget, protected QOpenGLFunctions
{
  OpenGLWidget (QWidget * = Q_NULLPTR, Qt::WindowFlags =Qt::WindowFlags());
  virtual ~OpenGLWidget ();
  
  void setOrtho ();
  void setPerspective ();
  
  void setPitch (double);
  void setYaw (double);
  
  QSize minimumSizeHint () const Q_DECL_OVERRIDE;
  QSize sizeHint () const Q_DECL_OVERRIDE;
  
protected:
  virtual void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
  virtual void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
  virtual void wheelEvent (QWheelEvent* event) Q_DECL_OVERRIDE;
  
  virtual void initializeGL () Q_DECL_OVERRIDE;
  virtual void resizeGL (int, int) Q_DECL_OVERRIDE ;
  virtual void paintGL () Q_DECL_OVERRIDE;
  
  virtual double velocity (double) const;
  virtual double zoom () const = 0;
  virtual void   zoom (bool) = 0;
  virtual void   draw () = 0;
  
  static QVector3D sphericToCarthesian (double lat, double lon);
  
  OpenGLTexturePointer texture(QImage);
  
  QMatrix4x4 m_projection, m_modelview;
  QVector3D  m_bgcolor;
  QVector3D  m_observer;
  
  double     m_pitch, m_yaw;
  
  bool       m_ortho;
  
  bool       m_mouse_tracking_active;
  int        m_mouse_x, m_mouse_y;
};
