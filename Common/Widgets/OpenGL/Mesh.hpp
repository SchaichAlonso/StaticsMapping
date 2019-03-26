#pragma once

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QSharedPointer>
#include <QtGui/QVector2D>
#include <QtGui/QVector3D>
#include <QtGui/QVector4D>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLTexture>

#include <Widgets/OpenGLWidget.hpp>

#include "DrawElements.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace OpenGL
{
  typedef QSharedPointer<QOpenGLBuffer> QOpenGLBufferPointer;
  
  struct Vertex
  {
    Vertex();
    Vertex(QVector3D position, QVector4D color, QVector3D normal, QVector2D texcoord);
  
    static int positionOffset();
    static int colorOffset();
    static int normalOffset();
    static int texcoordOffset();
    
    QVector3D position;
    QVector4D color;
    QVector3D normal;
    QVector2D texcoord;
  };
  
  struct Geometry
  {
    Geometry();
    int addVertex(const Vertex &v);
    void setVertex(int index, const Vertex &v);
    void setVertexCount(int n);
    
    void bind(ShaderPointer shader);
    void release(ShaderPointer shader);
    
  protected:
    QVector<Vertex> m_data;
    QOpenGLBuffer m_buffer;
    
    bool m_invalid;
  };
  
  struct Mesh
  {
    Mesh();
    virtual ~Mesh();
    
    void draw(ShaderPointer shader);
    void bind(ShaderPointer shader);
    void release(ShaderPointer shader);
    
    int addVertex(QVector3D coords, QVector3D normal, QVector2D texcoord);
    int addVertex(QVector3D coords, QVector3D color);
    int addVertex(QVector3D coords, QVector4D color);
    void setVertex(int index, QVector3D coords, QVector3D normal, QVector2D texcoord);
    void setVertex(int index, QVector3D coords, QVector4D color);
    void setVertexCount(int n);
    void drawElements(DrawElementsPointer draw);
    void drawElements(DrawElements *draw);
    
  protected:
    QList<DrawElementsPointer> m_draw_elements;
    Geometry          m_geometry;
  };
  
  typedef QSharedPointer<Mesh> MeshPointer;
}
