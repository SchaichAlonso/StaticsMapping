#include <QtMath>

#include "ZoomableCamera.hpp"

namespace OpenGL
{
  ZoomableCamera::ZoomableCamera(QRect screen, QString name, Zoom min, Zoom max, bool logarithmic)
  : Camera(name)
  , m_screen(screen)
  , m_min(min)
  , m_max(max)
  , m_cur((max - min) / 2 + min)
  , m_logarithmic(logarithmic)
  {
  }
  
  
  ZoomableCamera::~ZoomableCamera()
  {
  }
  
  ZoomableCamera::Zoom ZoomableCamera::defaultMinimalZoom()
  {
    return 1;
  }
  
  ZoomableCamera::Zoom ZoomableCamera::defaultMaximumZoom()
  {
    return 512;
  }
  
  
  ZoomableCamera::Zoom ZoomableCamera::minimalZoom() const
  {
    return (m_min);
  }
  
  
  void ZoomableCamera::setMinimalZoom(Zoom min)
  {
    m_min = min;
  }
  
  
  ZoomableCamera::Zoom ZoomableCamera::maximumZoom() const
  {
    return (m_max);
  }
  
  
  void ZoomableCamera::setMaximum(Zoom max)
  {
    m_max = max;
  }
  
  
  void ZoomableCamera::zoom(bool in)
  {
    Zoom z(in? -1 : 1);
    Zoom n;
    
    if (m_logarithmic) {
      n = qExp(qLn(m_cur) + 0.25*z);
    } else {
      n = m_cur + z;
    }
    
    n = qMax(m_min, qMin(m_max, n));
    
    qDebug() << "zoom from " << m_cur << " to " << n;
    m_cur = n;
  }
  
  
  ZoomableCamera::Zoom ZoomableCamera::zoom() const
  {
    return (m_cur);
  }
  
  
  void ZoomableCamera::setZoom(Zoom zoom)
  {
    m_cur = zoom;
  }
  
  
  bool ZoomableCamera::logarithmicZoom() const
  {
    return (m_logarithmic);
  }
  
  
  void ZoomableCamera::setLogarithmicZoom(bool enable)
  {
    m_logarithmic = enable;
  }
  
  void  ZoomableCamera::setScreen(QRect rect)
  {
    m_screen = rect;
  }
  
  
  QRect ZoomableCamera::screen() const
  {
    return (m_screen);
  }
}
