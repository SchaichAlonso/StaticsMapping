#include "RawImage.hpp"

namespace OpenGL
{
  RawImage::RawImage(Data data, QSize size)
  : m_data(data)
  , m_size(size)
  {
  }
  
  RawImage::RawImage(QSize size)
  : RawImage(Data(size.isValid()? (size.width() * size.height()) : 0), size)
  {
  }
  
  bool RawImage::isValid() const
  {
    return (m_size.isValid());
  }
  
  RawImage::Data RawImage::data() const
  {
    return (m_data);
  }
  
  QSize RawImage::size() const
  {
    return (m_size);
  }
  
  const RawImage::Pixel &RawImage::pixel(int x, int y) const
  {
    return (m_data[y * m_size.width() + x]);
  }
  
  void RawImage::setPixel(int x, int y, const Pixel &pixel)
  {
    m_data[y * m_size.width() + x] = pixel;
  }
  
  void RawImage::setPixel(int x, int y, const QColor &color)
  {
    setPixel(x, y, QVector4D(color.redF(), color.greenF(), color.blueF(), color.alphaF()));
  }
}
