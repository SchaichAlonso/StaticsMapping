#include <cstddef>

#include "Mesh.hpp"
#include "Obj8Shader.hpp"

#define _off(Type,Member) \
  reinterpret_cast<ptrdiff_t>(reinterpret_cast<char *>(&reinterpret_cast<Type*>(0)->Member))

namespace OpenGL
{
  
  Vertex::Vertex()
  : position()
  , color()
  , normal()
  , texcoord()
  {
  }
  
  Vertex::Vertex(QVector3D position, QColor color, QVector3D normal, QVector2D texcoord)
  : Vertex(position, QVector4D(color.redF(), color.greenF(), color.blueF(), color.alphaF()), normal, texcoord)
  {
  }
  
  Vertex::Vertex(QVector3D position, QVector4D color, QVector3D normal, QVector2D texcoord)
  : position(position)
  , color(color)
  , normal(normal)
  , texcoord(texcoord)
  {
  }
  
  Geometry::Geometry()
  : m_data(QOpenGLBuffer::VertexBuffer)
  , m_invalid(true)
  {
  }
  
  
  int
  Geometry::addVertex(const Vertex &v)
  {
    int index(this->m_data.size());
    setVertexCount(index+1);
    setVertex(index, v);
    return (index);
  }
  
  
  void
  Geometry::setVertex(int index, const Vertex &v)
  {
    if (index >= this->m_data.size()) {
      throw std::runtime_error("Index out of Bounds");
    }
    
    m_data[index] = v;
    m_invalid = true;
  }
  
  
  void
  Geometry::setVertexCount(int n)
  {
    m_data.resize(n);
  }
  
  template<typename Data>
  void writeToBuffer(QOpenGLBuffer &buffer, const QVector<Data> &vector)
  {
    if (buffer.isCreated()) {
      buffer.destroy();
    }
    buffer.create();
    buffer.allocate(vector.constData(), vector.size() * sizeof(vector[0]));
  }
  
  void
  Geometry::bind(ShaderPointer shader)
  {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    const Vertex *v(m_data.constData());
    GLsizei stride(sizeof(*v));
    
    if (m_invalid) {
      if (m_buffer.isCreated()) {
        m_buffer.destroy();
      }
      m_buffer.create();
      m_buffer.bind();
      m_buffer.allocate(v, m_data.size() * stride);
      m_invalid = false;
    }
    
    if (m_buffer.isCreated()) {
      m_buffer.bind();
      shader->enableAttributeArray("a_position");
      shader->enableAttributeArray("a_normal");
      shader->enableAttributeArray("a_texcoord");
      shader->enableAttributeArray("a_color");
      
      shader->setAttributeBuffer("a_position", GL_FLOAT, _off(Vertex, position), 3, stride);
      shader->setAttributeBuffer("a_normal",   GL_FLOAT, _off(Vertex, normal),   3, stride);
      shader->setAttributeBuffer("a_texcoord", GL_FLOAT, _off(Vertex, texcoord), 2, stride);
      shader->setAttributeBuffer("a_color",    GL_FLOAT, _off(Vertex, color),    4, stride);
    } else {
      glVertexPointer(3, GL_FLOAT, stride, &v->position);
      glColorPointer(4, GL_FLOAT, stride, &v->color);
      glNormalPointer(GL_FLOAT, stride, &v->normal);
      glTexCoordPointer(2, GL_FLOAT, stride, &v->texcoord);
    }
  }
  
  
  void
  Geometry::release(ShaderPointer shader)
  {
    if (m_buffer.isCreated()) {
      shader->disableAttributeArray("a_position");
      shader->disableAttributeArray("a_normal");
      shader->disableAttributeArray("a_texcoord");
      shader->disableAttributeArray("a_color");
    } else {
      glDisableClientState(GL_VERTEX_ARRAY);
      glDisableClientState(GL_COLOR_ARRAY);
      glDisableClientState(GL_NORMAL_ARRAY);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
  }
  
  
  Mesh::Mesh()
  : m_draw_elements()
  , m_geometry()
  {
  }
  
  
  Mesh::~Mesh()
  {
  }
  
  
  void
  Mesh::drawElements(DrawElementsPointer draw)
  {
    m_draw_elements.append(draw);
  }
  
  
  
  void
  Mesh::drawElements(DrawElements *draw)
  {
    drawElements(DrawElementsPointer(draw));
  }
  
  
  
  void
  Mesh::draw(ShaderPointer shader)
  {
    Q_FOREACH(DrawElementsPointer ptr, m_draw_elements) {
      ptr->draw(shader);
    }
  }
  
  
  void
  Mesh::bind(ShaderPointer shader)
  {
    m_geometry.bind(shader);
  }
  
  
  void
  Mesh::release(ShaderPointer shader)
  {
    m_geometry.release(shader);
  }
  
  
  int
  Mesh::addVertex(const Vertex &v)
  {
    return m_geometry.addVertex(v);
  }
  
  
  int
  Mesh::addVertex(QVector3D coords, QVector3D normal, QVector2D texcoord)
  {
    return addVertex(Vertex(coords, QVector4D(1,1,1,1), normal, texcoord));
  }
  
  
  int
  Mesh::addVertex(QVector3D coords, QVector3D color)
  {
    return addVertex(Vertex(coords, QVector4D(color,1), QVector3D(0,1,0), QVector2D()));
  }
  
  
  int
  Mesh::addVertex(QVector3D coords, QVector4D color)
  {
    return addVertex(Vertex(coords, color, QVector3D(0,1,0), QVector2D()));
  }
  
  
  void
  Mesh::setVertex(int index, const Vertex &vertex)
  {
    m_geometry.setVertex(index, vertex);
  }
  
  
  void
  Mesh::setVertex(int index, QVector3D coords, QVector3D normal, QVector2D texcoord)
  {
    setVertex(index, Vertex(coords, QVector4D(1,1,1,1), normal, texcoord));
  }
  
  
  void
  Mesh::setVertex(int index, QVector3D coords, QVector4D color)
  {
    setVertex(index, Vertex(coords, color, QVector3D(0,1,0), QVector2D()));
  }
  
  
  void
  Mesh::setVertexCount(int n)
  {
    m_geometry.setVertexCount(n);
  }
}
