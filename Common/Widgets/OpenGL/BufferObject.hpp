#pragma once

#include <QtCore/QSharedPointer>
#include <QOpenGLBuffer>

#include "API.hpp"

namespace OpenGL {
  struct BufferObject {
    using UsagePattern = QOpenGLBuffer::UsagePattern;
    using Access = QOpenGLBuffer::Access;
    using RangeAccessFlag = QOpenGLBuffer::RangeAccessFlag;
    using RangeAccessFlags = QOpenGLBuffer::RangeAccessFlags;
    
    BufferObject();
    explicit BufferObject(GLenum type);
    BufferObject(const BufferObject &other);
    ~BufferObject();
  
    BufferObject &operator=(const BufferObject &other);
  
    GLenum type() const;
    static GLenum bindingType(GLenum type); 
  
    UsagePattern usagePattern() const;
    void setUsagePattern(UsagePattern value);
  
    bool create();
    bool isCreated() const;
  
    void destroy();
  
    bool bind();
    bool isBound() const;
    void release();
  
    GLuint bufferId() const;
    GLuint boundId() const;
  
    int size() const;
  
    bool read(int offset, void *data, int count);
    void write(int offset, const void *data, int count);
  
    void allocate(const void *data, int count);
    void allocate(int count);
  
    void *map(Access access);
    void *mapRange(int offset, int count, RangeAccessFlags access);
    bool unmap();
  
  protected:
    struct Data {
      Data(GLenum type);
      ~Data();
      
      bool create();
      void destroy();
      
      OpenGL::API *m_opengl;
      GLenum m_type;
      UsagePattern m_usage;
      GLuint m_id;
    };
    QSharedPointer<Data> m_impl;
  };
  
  typedef QSharedPointer<BufferObject> BufferObjectPointer;
}
