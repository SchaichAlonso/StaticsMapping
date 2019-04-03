#pragma once

#include <QtCore/QSize>
#include <QtCore/QVector>
#include <QtGui/QColor>
#include <QtGui/QVector4D>

namespace OpenGL
{
  struct RawImage
  {
    typedef QVector4D Pixel;
    typedef QVector<Pixel> Data;
    
    RawImage(Data data, QSize size);
    RawImage(QSize size=QSize());
    
    bool isValid() const;
    Data data() const;
    QSize size() const;
    
    const Pixel &pixel(int x, int y) const;
    void setPixel(int x, int y, const Pixel &color);
    void setPixel(int x, int y, const QColor &color);
    
  protected:
    Data m_data;
    QSize m_size;
  };
}
