#include "Std140Blob.hpp"

namespace OpenGL
{
  Std140Blob::Std140Blob()
  {
    memset(this, 0, sizeof(*this));
    dirty = true;
  }
  
  template<typename T> void Std140Blob::write(T &target, const T &value)
  {
    if (target != value) {
      target = value;
      dirty = true;
    }
  }
  
  bool Std140Blob::Lighting::Light::operator!= (const Light &other) const
  {
    if (this->position != other.position)
      return true;
    if (this->color != other.color)
      return true;
    if (this->spot.exponent != other.spot.exponent)
      return true;
    if (this->spot.direction != other.spot.direction)
      return true;
    if (this->spot.cutoffAngle != other.spot.cutoffAngle)
      return true;
    if (this->attenuation.exponent != other.attenuation.exponent)
      return true;
    if (this->attenuation.range != other.attenuation.range)
      return true;
    return false;
  }
  
  void Std140Blob::setModelview(const QMatrix4x4 &m)
  {
    write(transform.modelview, m.toGenericMatrix<4,4>());
  }
  
  void Std140Blob::setProjection(const QMatrix4x4 &m)
  {
    write(transform.projection, m.toGenericMatrix<4,4>());
  }
  
  void Std140Blob::setAmbient(QColor ambient)
  {
    write(lighting.ambient, QVector3D(ambient.redF(), ambient.greenF(), ambient.blueF()));
  }
  
  void Std140Blob::setLight(int n, LightPointer l)
  {
    Lighting::Light value{};
    
    value.position = l->position();
    value.color = QVector3D(l->color().redF(), l->color().greenF(), l->color().blueF());
    value.attenuation.exponent = l->rangeExp();
    value.attenuation.range = l->range();
    value.spot.cutoffAngle = l->spotCutoffAngle();
    value.spot.direction = l->spotDirection();
    value.spot.exponent = l->spotExp();
    
    write(lighting.light_data[n], value);
  }
  
  int Std140Blob::lights() const
  {
    return sizeof(lighting.light_data)/sizeof(*lighting.light_data);
  }
  
  int Std140Blob::activeLights() const
  {
    return lighting.light_count;
  }
  
  void Std140Blob::setActiveLights(int active)
  {
    write(lighting.light_count, qMin(active,lights()));
  }
  
  void Std140Blob::setTextureUnitEnabled(int unit, bool enabled)
  {
    write(state.texture_unit_enabled[unit].value, enabled?1:0);
  }
  
  void Std140Blob::setTexturingEnabled(bool enabled)
  {
    write(state.texturing_enabled, enabled?1:0);
  }
  
  void Std140Blob::setLightingEnabled(bool enabled)
  {
    write(state.lighting_enabled, enabled?1:0);
  }
  
  int Std140Blob::size()
  {
    /*
     * bother?
     */
#if 0
    return reinterpret_cast<int>(&reinterpret_cast<Std140Blob*>(0)->end_indicator+1);
#else
    return sizeof(Std140Blob);
#endif
  }
  
  void Std140Blob::upload(BufferObjectPointer ubo)
  {
    if (dirty) {
      end_indicator = 0x55aa55aa;
      ubo->bind();
      GLint ubo_size(ubo->size());
      Q_ASSERT(ubo_size >= size());
#if 0
      ubo->write(0, this, size());
#else
      void *p = ubo->map(QOpenGLBuffer::WriteOnly);
      memcpy(p, this, size());
      ubo->unmap();
#endif
      ubo->release();
      
      dirty = false;
    }
  }
  
  BufferObjectPointer Std140Blob::createBufferObject()
  {
    BufferObjectPointer retval(new BufferObject(GL_UNIFORM_BUFFER));
    
    retval->create();
    retval->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    retval->bind();
    retval->allocate(size());
    if (retval->size() < size()) {
      throw std::runtime_error("Could not allocate buffer object.");
    }
    retval->release();
    
    return (retval);
  }
}
