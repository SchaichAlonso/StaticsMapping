#include "BufferObject.hpp"

namespace OpenGL {
  BufferObject::Data::Data(GLenum type)
  : m_opengl(Q_NULLPTR)
  , m_type(type)
  , m_usage(QOpenGLBuffer::DynamicDraw)
  , m_id(0)
  {
  }
  
  BufferObject::Data::~Data()
  {
    destroy();
  }
  
  bool BufferObject::Data::create()
  {
    if (m_id == 0) {
      m_opengl = OpenGL::api();
      m_opengl->glGenBuffers(1, &m_id);
    }
    return (m_id != 0);
  }
  
  void BufferObject::Data::destroy()
  {
    if (m_id != 0) {
      m_opengl->glDeleteBuffers(1, &m_id);
      m_id = 0;
    }
  }

  BufferObject::BufferObject()
  : BufferObject(0)
  {
  }
  
  BufferObject::BufferObject(GLenum type)
  : m_impl(new Data(type))
  {
  }
  
  BufferObject::BufferObject(const BufferObject& other)
  : m_impl(other.m_impl)
  {
  }
  
  BufferObject::~BufferObject()
  {
  }
  
  BufferObject &BufferObject::operator=(const BufferObject &other)
  {
    if (m_impl != other.m_impl) {
      m_impl = other.m_impl;
    }
    return *this;
  }
  
  GLenum BufferObject::type() const
  {
    return m_impl->m_type;
  }
  
  GLenum BufferObject::bindingType(GLenum type)
  {
    /*
     * OpenGL 4.5
     */
#define CASE(name) case name: return name##_BINDING

    switch (type) {
      CASE(GL_ARRAY_BUFFER);
      CASE(GL_ATOMIC_COUNTER_BUFFER);
      CASE(GL_COPY_READ_BUFFER);
      CASE(GL_COPY_WRITE_BUFFER);
      CASE(GL_DRAW_INDIRECT_BUFFER);
      CASE(GL_DISPATCH_INDIRECT_BUFFER);
      CASE(GL_ELEMENT_ARRAY_BUFFER);
      CASE(GL_PIXEL_PACK_BUFFER);
      CASE(GL_PIXEL_UNPACK_BUFFER);
      CASE(GL_SHADER_STORAGE_BUFFER);
      CASE(GL_TRANSFORM_FEEDBACK_BUFFER);
      CASE(GL_UNIFORM_BUFFER);
    }
#undef CASE
    return -1;
  }
  
  BufferObject::UsagePattern BufferObject::usagePattern() const
  {
    return m_impl->m_usage;
  }
  
  void BufferObject::setUsagePattern(UsagePattern value)
  {
    m_impl->m_usage = value;
  }
  
  bool BufferObject::create()
  {
    return m_impl->create();
  }
  
  bool BufferObject::isCreated() const
  {
    return (m_impl->m_id != 0);
  }
  
  void BufferObject::destroy()
  {
    m_impl->destroy();
  }
  
  bool BufferObject::bind()
  {
    if (isCreated()) {
      m_impl->m_opengl->glBindBuffer(type(), bufferId());
      return true;
    }
    return false;
  }
  
  bool BufferObject::isBound() const
  {
    if (isCreated()) {
      return (boundId() == bufferId());
    }
    return false;
  }
  
  void BufferObject::release()
  {
    m_impl->m_opengl->glBindBuffer(type(), 0);
  }
  
  GLuint BufferObject::bufferId() const
  {
    return m_impl->m_id;
  }
  
  GLuint BufferObject::boundId() const
  {
    GLint id=-5;
    GLenum name{bindingType(type())};
    if (isCreated()) {
      m_impl->m_opengl->glGetIntegerv(name, &id);
    } else {
      OpenGL::api()->glGetIntegerv(name, &id);
    }
    return id;
  }
  
  int BufferObject::size() const
  {
    Q_ASSERT(isBound());
    
    GLint size;
    m_impl->m_opengl->glGetBufferParameteriv(GL_UNIFORM_BUFFER, GL_BUFFER_SIZE, &size);
    return size;
  }
  
  bool BufferObject::read(int offset, void *data, int count)
  {
    Q_ASSERT(isBound());
    
    while (true) { // Clear error state.
      GLenum error = m_impl->m_opengl->glGetError();
      if (error == GL_NO_ERROR)
        break;
      if (error == GL_CONTEXT_LOST)
        return false;
    };
    m_impl->m_opengl->glGetBufferSubData(m_impl->m_type, offset, count, data);
    return (m_impl->m_opengl->glGetError() == GL_NO_ERROR);
  }
  
  void BufferObject::write(int offset, const void *data, int count)
  {
    Q_ASSERT(isBound());
    
    m_impl->m_opengl->glBufferSubData(m_impl->m_type, offset, count, data);
  }
  
  void BufferObject::allocate(const void *data, int count)
  {
    Q_ASSERT(isCreated());
    Q_ASSERT(isBound());
    
    m_impl->m_opengl->glBufferData(m_impl->m_type, count, data, m_impl->m_usage);
  }
  
  void BufferObject::allocate(int count)
  {
    allocate(Q_NULLPTR, count);
  }
  
  void *BufferObject::map(Access access)
  {
    QOpenGLBuffer::RangeAccessFlags rangeAccess = 0;
    if (access == QOpenGLBuffer::ReadOnly || access == QOpenGLBuffer::ReadWrite) {
      rangeAccess |= QOpenGLBuffer::RangeRead;
    }
    if (access == QOpenGLBuffer::WriteOnly || access == QOpenGLBuffer::ReadWrite) {
      rangeAccess |= QOpenGLBuffer::RangeWrite;
    }
    
    return mapRange(0, size(), rangeAccess);
    //return m_impl->m_opengl->glMapBufferRange(m_impl->m_type, 0, size(), rangeAccess);
  }
  
  void *BufferObject::mapRange(int offset, int count, RangeAccessFlags access)
  {
    Q_ASSERT(isCreated());
    Q_ASSERT(isBound());
    return m_impl->m_opengl->glMapBufferRange(m_impl->m_type, offset, count, access);
  }
  
  bool BufferObject::unmap()
  {
    return m_impl->m_opengl->glUnmapBuffer(m_impl->m_type) == GL_TRUE;
  }
}
