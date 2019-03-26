#include <cstddef>
#include <ciso646>

#include <QtCore/QtMath>
#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>

#include "OpenGL/Context.hpp"
#include "OpenGLWidget.hpp"

namespace Widgets
{
  OpenGLWidget::OpenGLWidget(QWidget *parent, Qt::WindowFlags flags)
  : QOpenGLWidget(parent, flags)
  , QOpenGLFunctions()
  , m_t0(QDateTime::currentDateTimeUtc())
  , m_projection()
  , m_modelview()
  , m_bgcolor(0, 0, 0)
  , m_observer(0, 0, 1)
  , m_offscreen()
  , m_pitch(0)
  , m_yaw(0)
  , m_ortho(true)
  , m_mouse_tracking_active(false)
  {
    
    /*QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    fmt.setSamples(128);
    setFormat(fmt);*/
  }
  
  
  
  OpenGLWidget::~OpenGLWidget()
  {
  }
  
  
  
  void
  OpenGLWidget::setOrtho()
  {
    m_ortho = true;
  }
  
  
  
  void 
  OpenGLWidget::setPerspective()
  {
    m_ortho = false;
  }
  
  
  
  void
  OpenGLWidget::setPitch(double angle)
  {
    angle = angle >  89 ?  89 : angle;
    angle = angle < -89 ? -89 : angle;
    
    m_pitch = angle;
    
    update();
  }
  
  
  
  void
  OpenGLWidget::setYaw(double angle)
  {
    m_yaw = angle;
    
    update();
  }
  
  
  QSize
  OpenGLWidget::minimumSizeHint() const
  {
    return (QSize(256, 256));
  }
  
  
  
  QSize
  OpenGLWidget::sizeHint() const
  {
    return (QSize(512, 512));
  }
  
  
  
  void
  OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
  {
    int x = event->x();
    int y = event->y();
    
    double dx = velocity(m_mouse_x - x);
    double dy = velocity(m_mouse_y - y);
    
    m_mouse_x = x;
    m_mouse_y = y;
    
    if (not m_ortho) {
      setYaw(m_yaw + dx);
      setPitch(m_pitch - dy);
    }
    
    //QWidget::mouseMoveEvent (event);
  }
  
  
  
  void
  OpenGLWidget::mousePressEvent(QMouseEvent* event)
  {
    m_mouse_x = event->x();
    m_mouse_y = event->y();
    
    //QWidget::mousePressEvent (event);
  }
  
  
  
  void
  OpenGLWidget::wheelEvent(QWheelEvent* event)
  {
    int delta = event->delta();
    
    if (event->inverted()) {
      delta = -delta;
    }
    
    if (delta != 0) {
      zoom(delta<0);
    }
    
    update();
  }
  
  
  
  void
  OpenGLWidget::initializeGL()
  {
    initializeOpenGLFunctions();
    
    glClearDepth(1);
    glDepthRange(0, 1);
  }
  
  
  
  void
  OpenGLWidget::resizeGL(int w, int h)
  {
#if 0
    glViewport(0, 0, w, h);
#endif
    
    update();
  }
  
  
  
  void
  OpenGLWidget::paintGL()
  {
    m_projection.setToIdentity();
    m_modelview.setToIdentity();
    
    double aspect = ((double)width()) / ((double)height());
    double d = zoom();
    
    if (m_ortho) {
      m_projection.ortho(
        -d/2, d/2,
        -d/(2*aspect), d/(2*aspect),
        -500, 500
      );
      m_modelview.rotate(m_pitch, 1, 0, 0);
      m_modelview.rotate(m_yaw, 0, 1, 0);
      
      m_observer = QVector3D(0, 0, 1);
    } else {
      m_projection.perspective(45, aspect, 0.01f, 1000);
      
      m_observer = d * sphericToCarthesian(m_pitch, m_yaw);
      
      m_modelview.lookAt(
        m_observer,
        QVector3D (0, 0, 0),
        QVector3D (0, 1, 0)
      );
    }
        
#if 0
    GLint max_viewport_dimensions[2];
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, &max_viewport_dimensions[0]);
    
    QSize viewport(
      qMin<int>(qNextPowerOfTwo(width()), max_viewport_dimensions[0]),
      qMin<int>(qNextPowerOfTwo(height()), max_viewport_dimensions[1])
    );
#else
    QSize viewport(size());
#endif
    m_offscreen.bind(viewport);
    glViewport(0, 0, viewport.width(), viewport.height());
    
    glClearColor(m_bgcolor[0], m_bgcolor[1], m_bgcolor[2], 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m_projection.constData());
    
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(m_modelview.constData());
    
    draw();
    
    m_offscreen.release();
    m_offscreen.blit(Q_NULLPTR, rect());
    
    QDateTime t1(QDateTime::currentDateTimeUtc());
    
    if (t1 != m_t0) {
      qint64 msecs(m_t0.msecsTo(t1));
      if (msecs != 0) {
        qDebug("Took %d msecs (%f fps)", int(msecs), 1000.0 / msecs);
      }
      m_t0 = t1;
    }
  }
  
  
  
  double
  OpenGLWidget::velocity(double v) const
  {
    return (v);
  }
  
  
  
  QVector3D
  OpenGLWidget::sphericToCarthesian(float lat, float lon)
  {
    float cos_lat, cos_lon, sin_lat, sin_lon;
    cos_lon = qCos(qDegreesToRadians(lon));
    sin_lon = qSin(qDegreesToRadians(lon));
    cos_lat = qCos(qDegreesToRadians(lat));
    sin_lat = qSin(qDegreesToRadians(lat));
    
    return (
      QVector3D(
        cos_lat*sin_lon,
        sin_lat,
        cos_lat*cos_lon
      )
    );
  }
}
