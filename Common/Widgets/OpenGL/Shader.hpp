#pragma once

#include <QtCore/QMap>
#include <QtCore/QSharedPointer>

#include <QtGui/QMatrix4x4>
#include <QtGui/QVector2D>
#include <QtGui/QVector3D>
#include <QtGui/QVector4D>
#include <QtGui/QOpenGLShaderProgram>

#include "Light.hpp"

namespace OpenGL
{
  struct Shader
  {
    Shader(QString vertex, QString fragment);
    virtual ~Shader();
    
    void setTextureUnitEnabled(int unit, bool enabled);
    void setTexturingEnabled(bool enable);
    void setLightingEnabled(bool enable);
    void setLights(QList<LightPointer> lights);
    void setModelviewMatrix(const QMatrix4x4& value);
    void setProjectionMatrix(const QMatrix4x4& value);
    
    void bind();
    void release();
    
    void enableAttributeArray(QString name);
    void disableAttributeArray(QString name);
    void setAttributeBuffer(QString name, GLenum type, int offset, int tupleSize, int stride);
    
    void setUniformValue(QString name, GLfloat value);
    void setUniformValue(QString name, GLint value);
    void setUniformValue(QString name, const QVector2D& value);
    void setUniformValue(QString name, const QVector3D& value);
    void setUniformValue(QString name, const QVector4D& value);
    void setUniformValue(QString name, const QMatrix4x4& value);
    
  protected:
    typedef QSharedPointer<QOpenGLShaderProgram> ImplementationPointer;
    ImplementationPointer m_impl;
    
    ImplementationPointer createShader() const;
    void setUniforms(ImplementationPointer shader) const;
    
  protected:
    QString m_vert_shader_filename;
    QString m_frag_shader_filename;
    
    QMap<QString, GLfloat>    m_floats;
    QMap<QString, GLint>      m_ints;
    QMap<QString, QVector2D>  m_vec2s;
    QMap<QString, QVector3D>  m_vec3s;
    QMap<QString, QVector4D>  m_vec4s;
    QMap<QString, QMatrix4x4> m_mat4x4s;
  };

  typedef QSharedPointer<Shader> ShaderPointer;
}
