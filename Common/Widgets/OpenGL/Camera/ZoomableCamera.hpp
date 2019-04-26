#pragma once

#include <QtCore/QRect>

#include "Camera.hpp"

namespace OpenGL
{
  struct ZoomableCamera : Camera
  {
    ZoomableCamera(QRect screen, QString name=QString(), Zoom min=defaultMinimalZoom(), Zoom max=defaultMaximumZoom(), bool logarithmic=true);
    virtual ~ZoomableCamera();
    
    static Zoom defaultMinimalZoom();
    static Zoom defaultMaximumZoom();
    
    Zoom minimalZoom() const;
    void setMinimalZoom(Zoom min);
    
    Zoom maximumZoom() const;
    void setMaximum(Zoom max);
    
    void zoom(bool in) Q_DECL_OVERRIDE;
    Zoom zoom() const Q_DECL_OVERRIDE;
    void setZoom(Zoom zoom);
    
    bool logarithmicZoom() const;
    void setLogarithmicZoom(bool enable);
  
    void  setScreen(QRect rect);
    QRect screen() const;
    
  protected:
    QRect m_screen;
    Zoom m_min, m_max, m_cur;
    bool m_logarithmic;
  };
}
