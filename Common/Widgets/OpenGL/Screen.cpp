#include <ciso646>

#include <QtCore/QtMath>
#include <QtCore/QTimer>
#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QOpenGLFunctions_3_3_Core>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <QtGui/QPalette>

#include <Common/DataPath.hpp>

#include "API.hpp"
#include "Screen.hpp"
#include "Camera/PerspectiveCamera.hpp"
#include "Camera/OrthoCamera.hpp"
#include "Camera/OnScreenDisplay.hpp"
#include "State/PolygonMode.hpp"

namespace OpenGL
{
  View::View(QRectF relative, CameraPointer camera)
  : m_relative(relative)
  , m_camera(camera)
  {
  }
  
  
  CameraPointer View::camera() const
  {
    return (m_camera);
  }
  
  
  QRect View::viewport(QRect screen) const
  {
    return QRect(
      screen.left() + screen.width()  * m_relative.left(),
      screen.top() + screen.height() * m_relative.top(),
      screen.width()  * m_relative.width(),
      screen.height() * m_relative.height()
    );
  }
  
  
  Screen::Screen(ScenePointer scene)
  : m_views()
  , m_scene(scene)
  , m_active()
  , m_last_cursor_pos()
  , m_last_frame_completion(QDateTime::currentDateTimeUtc())
  , m_fbo(QOpenGLTexture::RGBA32F)
  , m_hdr(false)
  , m_benchmark(false)
  {
#if 0
    m_views << View(QRectF(0.0, 0.0, 1.0, 1.0), CameraPointer(new PerspectiveCamera(30, "full")));
#else
    m_views << View(QRectF(0.0, 0.0, 0.5, 0.5), CameraPointer(new PerspectiveCamera(30, "topleft")));
    m_views << View(QRectF(0.5, 0.0, 0.5, 0.5), CameraPointer(new OrthoCamera("topright")));
    m_views << View(QRectF(0.0, 0.5, 0.5, 0.5), CameraPointer(new OrthoCamera("bottomleft")));
    m_views << View(QRectF(0.5, 0.5, 0.5, 0.5), CameraPointer(new OrthoCamera("bottomright")));
    
    m_views[2].camera()->setYaw(90);
    m_views[3].camera()->setPitch(-90);
#endif
    setMinimumSize(QSize(256,256));
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  }
  
  
  
  Screen::~Screen()
  {
    makeCurrent();
    m_scene.reset();
    doneCurrent();
  }
  
  
  
  CameraPointer
  Screen::cameraAt(QPoint pos) const
  {
    QRect screen(rect());
    QPoint gl_pos(pos.x(), screen.height() - pos.y());
    Q_FOREACH(View v, m_views) {
      QRect viewport(v.viewport(screen));
      if (viewport.contains(gl_pos)) {
        return (v.camera());
      }
    }
    return CameraPointer();
  }
  
  
  void
  Screen::keyReleaseEvent(QKeyEvent *event)
  {
    if (event->key() == 'H') {
      m_hdr = !m_hdr;
      event->accept();
    }
    
    if (!event->isAccepted()) {
      QOpenGLWidget::keyReleaseEvent(event);
    }
  }
  
  
  
  void
  Screen::mouseMoveEvent(QMouseEvent *event)
  {
    if (m_active) {
      if(not m_active->ortho()) {
        QPoint movement(event->pos() - m_last_cursor_pos);
        m_last_cursor_pos = event->pos();
        
        float pitch(m_active->pitch() + movement.y());
        pitch = qMax(pitch, -89.9f);
        pitch = qMin(pitch, +89.9f);
        m_active->setPitch(pitch);
        m_active->setYaw(m_active->yaw() - movement.x());
      }
    }
    update();
  }
  
  
  
  void
  Screen::mousePressEvent(QMouseEvent *event)
  {
    m_last_cursor_pos = event->pos();
    m_active = cameraAt(m_last_cursor_pos);
  }
  
  
  
  void
  Screen::mouseReleaseEvent(QMouseEvent *event)
  {
    Q_UNUSED(event);
    m_active.reset();
  }
  
  
  
  void
  Screen::wheelEvent(QWheelEvent* event)
  {
    CameraPointer active(m_active);
    if (active.isNull()) {
      active = cameraAt(event->pos());
    }
    if (active) {
      int delta((event->inverted()? -1:1) * event->delta());
      active->zoom(delta < 0);
    }
    
    update();
  }
  
  
  
  void
  Screen::initializeGL()
  {
    API *gl{OpenGL::api()};
    
    gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gl->glEnable(GL_BLEND);
  }
  
  
  
  void
  Screen::resizeGL(int w, int h)
  {
    Q_UNUSED(w);
    Q_UNUSED(h);
    
    Q_FOREACH(View view, m_views) {
      QRect screen(view.viewport(rect()));
      qSharedPointerCast<ZoomableCamera>(view.camera())->setScreen(QRect(QPoint(0,0), screen.size()));
    }
    
    update();
  }
  
  
  
  void
  Screen::paintGL()
  {
    //QColor bg(QPalette().color(QPalette::Window));
    QColor bg(Qt::black);
    
    OpenGL::API *gl{OpenGL::api()};
    
    gl->glEnable(GL_DEPTH_TEST);
    gl->glClearColor(bg.redF(), bg.greenF(), bg.blueF(), 0);
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    QImage osd;
    if (m_benchmark) {
      osd = QImage(size(), QImage::Format_RGBA8888);
      osd.fill(QColor(0,0,0,0));
    }
    
    Q_FOREACH(View view, m_views) {
      QRect screen(view.viewport(rect()));
      QRect viewport(screen);
      int   ssaa(1);
      CameraPointer camera(view.camera());
      
      if (m_hdr) {
        ssaa = 1;
        viewport = QRect(QPoint(0,0), clampedViewport(ssaa*viewport.size()));
        m_fbo.bind(viewport.size());
      }
      
      gl->glViewport(viewport.x(), viewport.y(), viewport.width(), viewport.height());
      
      if (m_hdr) {
        gl->glClearColor(0, 0, 0, 0);
        gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      }
      gl->glLineWidth(ssaa);
    
      camera->setPosition(QVector3D(0, 0, camera->zoom()));
      camera->setScreenDimensions(viewport.size());
      m_scene->draw(camera);
    
      if (m_hdr) {
        m_fbo.release();
#if 0   
        QImage fbo(m_fbo.handle()->toImage());
        QImage scaled(fbo.scaled(screen.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        //QImage scaled(fbo);
        //fbo.save(QString("/tmp/fbo-raw-%1.jpg").arg(camera->name()));
        //scaled.save(QString("/tmp/fbo-scaled-%1.jpg").arg(camera->name()));
        //gl->glViewport(0, 0, width(), height());
        //osdScene(screen, scaled)->draw(CameraPointer(new OnScreenDisplay(rect())));
      
        QPainter painter(&osd);
        painter.drawImage(screen, scaled);
#else 
        m_fbo.blit(screen);
#endif
      }
    }
    
    QDateTime now(QDateTime::currentDateTimeUtc());
    QDateTime target(m_last_frame_completion.addMSecs(1000 / 30));
    qint64 delta_t(m_last_frame_completion.msecsTo(now));
    QString fps_label(QString("Rendered in %1 msecs\nFPS: %2\nHDR: %3").arg(delta_t).arg(1000.0/delta_t).arg(m_hdr));
    
    if(m_benchmark)
    {
      QPainter painter(&osd);
      painter.setPen(QColor(Qt::white));
      painter.drawText(osd.rect(), 0, fps_label);
    }
    m_last_frame_completion = now;
    
    if(m_benchmark) {
      gl->glViewport(0, 0, width(), height());
      osdScene(rect(), osd)->draw(CameraPointer(new OnScreenDisplay(rect())));
    
      if (now.msecsTo(target) <= 0) {
        update();
      } else {
        QTimer::singleShot(now.msecsTo(target), this, SLOT(update()));
      }
    } else {
      qDebug() << fps_label;
    }
  }
  
  
  
  QSize
  Screen::maxViewportDimensions() const
  {
    GLint buf[2] = {0, 0};
    if (isValid()) {
      OpenGL::api()->glGetIntegerv(GL_MAX_VIEWPORT_DIMS, &buf[0]);
    }
    return QSize(buf[0], buf[1]);
  }
  
  
  
  QSize Screen::clampedViewport(QSize candidate) const
  {
    QSize limit(maxViewportDimensions());
    
    return QSize(
      qMin(candidate.width(), limit.width()),
      qMin(candidate.height(), limit.height())
    );
  }
  
  
  SceneWeakPointer Screen::scene() const
  {
    return (m_scene);
  }
  
  
  
  OpenGL::ScenePointer
  Screen::osdScene(QRect target, QImage content)
  {
    OpenGL::ScenePointer scene{new OpenGL::Scene{m_scene->defaultShader()}};
    scene->insertModel(osdQuad(target, content));
    return (scene);
  }
  
  
  
  OpenGL::ModelPointer
  Screen::osdQuad(QRect dst, QImage content)
  {
    /*
     * QRect::right and QRect::bottom return off-by-one values.
     */
    dst.adjust(0, 0, 1, 1);
    
    ModelPointer model(new Model(Model::Texturing{true},Model::Lighting{false}));
    MeshPointer mesh(model->mesh());
    
    QVector3D normal(0,1,0);
    mesh->drawElements(
      new OpenGL::DrawElements(
        QVector<int>()
          << mesh->addVertex(QVector3D(dst.left(),  dst.bottom(), 0), normal, QVector2D(0, 1))
          << mesh->addVertex(QVector3D(dst.left(),  dst.top(),    0), normal, QVector2D(0, 0))
          << mesh->addVertex(QVector3D(dst.right(), dst.top(),    0), normal, QVector2D(1, 0))
          << mesh->addVertex(QVector3D(dst.left(),  dst.bottom(), 0), normal, QVector2D(0, 1))
          << mesh->addVertex(QVector3D(dst.right(), dst.top(),    0), normal, QVector2D(1, 0))
          << mesh->addVertex(QVector3D(dst.right(), dst.bottom(), 0), normal, QVector2D(1, 1)),
        GL_TRIANGLES
      )
    );
    
    model->setTexture(0, content);
    
    return (model);
  }
}
