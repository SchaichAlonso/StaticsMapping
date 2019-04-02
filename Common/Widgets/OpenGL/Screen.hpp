#pragma once

#include <QtCore/QList>
#include <QtCore/QPoint>
#include <QtCore/QRect>

#include <QtWidgets/QOpenGLWidget>

#include "Camera/Camera.hpp"
#include "FrameBufferObject.hpp"
#include "Model.hpp"
#include "Scene.hpp"

namespace OpenGL
{
  struct View
  {
    View(QRectF relative =QRectF(), CameraPointer camera =CameraPointer());
    
    CameraPointer camera() const;
    QRect viewport(QRect screen) const;
    
  protected:
    QRectF m_relative;
    CameraPointer m_camera;
  };
    
  struct Screen : QOpenGLWidget, protected QOpenGLFunctions
  {
    Screen(ScenePointer scene, bool hdr);
    virtual ~Screen();
    
    void addModel(ModelPointer model);
    
    static ScenePointer gridScene();
    static ScenePointer osdScene(QRect dst, QImage image);
    
  protected:
    CameraPointer cameraAt(QPoint point) const;
    
  protected:
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent (QWheelEvent *event) Q_DECL_OVERRIDE;
    
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE ;
    void paintGL() Q_DECL_OVERRIDE;
    
    QSize maxViewportDimensions() const;
    QSize clampedViewport(QSize candidate) const;
    
  protected:
    static ModelPointer localeIndicator();
    static ModelPointer grid();
    static ModelPointer osdQuad(QRect dst, QImage content);
    
  protected:
    QList<View> m_views;
    ScenePointer m_scene;
    
    CameraPointer m_active;
    QPoint m_last_cursor_pos;
    
    FrameBufferObject m_fbo;
    bool m_hdr;
  };
}
