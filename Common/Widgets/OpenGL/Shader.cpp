#include <stdexcept>

#include "API.hpp"
#include "Shader.hpp"

namespace OpenGL
{
  Shader::Shader(QString vertex, QString fragment)
  : m_impl()
  , m_ubo()
  , m_std140_blob()
  , m_vert_shader_filename(vertex)
  , m_frag_shader_filename(fragment)
  {
  }
  
  Shader::~Shader()
  {
  }
  
  
  void Shader::setTextureUnitEnabled(int unit, bool enabled)
  {
    m_std140_blob.setTextureUnitEnabled(unit, enabled);
  }
  
  
  void Shader::setTexturingEnabled(bool enable)
  {
    m_std140_blob.setTexturingEnabled(enable);
  }
  
  
  void Shader::setLightingEnabled(bool enable)
  {
    m_std140_blob.setLightingEnabled(enable);
  }
  
  
  void Shader::setLights(QList<LightPointer> lights)
  {
    m_std140_blob.setActiveLights(lights.size());
    for(int i=0; i<m_std140_blob.activeLights(); ++i) {
      m_std140_blob.setLight(i, lights[i]);
    }
  }
  
  void Shader::setAmbientColor(QColor ambient)
  {
    m_std140_blob.setAmbient(ambient);
  }
  
  void Shader::setModelviewMatrix(const QMatrix4x4 &value)
  {
    m_std140_blob.setModelview(value);
  }
  
  void Shader::setProjectionMatrix(const QMatrix4x4 &value)
  {
    m_std140_blob.setProjection(value);
  }
  
  
  
  QSharedPointer<QOpenGLShaderProgram> Shader::createShader() const
  {
    ImplementationPointer shader(new QOpenGLShaderProgram);
    
    bool ok(true);
    
    ok = ok && shader->addShaderFromSourceFile(
      QOpenGLShader::Vertex,
      m_vert_shader_filename
    );
    ok = ok && shader->addShaderFromSourceFile(
      QOpenGLShader::Fragment,
      m_frag_shader_filename
    );
    ok = ok && shader->link();
    
    ok = ok && shader->bind();
    
    if (ok) {
      setUniforms(shader);
    } else {
      throw std::runtime_error(QString("Failed to create Shader <%1>").arg(shader->log()).toStdString());
    }
    
    return (shader);
  }
  
#define SET_UNIFORM_VALUE(type, name, value) \
  if (m_impl) { \
    m_impl->setUniformValue(m_impl->uniformLocation(name), value); \
  } else { \
    m_##type##s[name] = value; \
  }
  
#define SET_UNIFORMS(type) \
  Q_FOREACH(QString name, m_##type##s.keys()) { \
    impl->setUniformValue(impl->uniformLocation(name), m_##type##s[name]); \
  }
  
  void Shader::setUniforms(ImplementationPointer impl) const
  {
    SET_UNIFORMS(float);
    SET_UNIFORMS(int);
    SET_UNIFORMS(vec2);
    SET_UNIFORMS(vec3);
    SET_UNIFORMS(vec4);
    SET_UNIFORMS(mat4x4);
  }
  
  void Shader::flushUniforms()
  {
    m_std140_blob.upload(m_ubo);
  }
  
  void Shader::bind()
  {
    if (m_impl.isNull()) {
      m_impl = createShader();
      m_ubo = m_std140_blob.createBufferObject();
      
      for (int texture_unit=0; texture_unit<8; ++texture_unit) {
        setUniformValue(QString("texture_unit[%1]").arg(texture_unit), texture_unit);
      }
      
      GLuint index = OpenGL::api()->glGetUniformBlockIndex(m_impl->programId(), "Std140Blob");
      OpenGL::api()->glBindBufferBase(GL_UNIFORM_BUFFER, index, m_ubo->bufferId());
    } else {
      m_impl->bind();
    }
  }
  
  void Shader::release()
  {
    m_impl->release();
  }
  
  void Shader::enableAttributeArray(QString name)
  {
    int a(m_impl->attributeLocation(name));
    m_impl->enableAttributeArray(a);
  }
  
  void Shader::disableAttributeArray(QString name)
  {
    int a(m_impl->attributeLocation(name));
    m_impl->disableAttributeArray(a);
  }
  
  void Shader::setAttributeBuffer(QString name, GLenum type, int offset, int tupleSize, int stride)
  {
    m_impl->setAttributeBuffer(qUtf8Printable(name), type, offset, tupleSize, stride);
  }
  
  void Shader::setUniformValue(QString name, GLfloat value)
  {
    SET_UNIFORM_VALUE(float, name, value);
  }
  
  void Shader::setUniformValue(QString name, GLint value)
  {
    SET_UNIFORM_VALUE(int, name, value);
  }
  
  void Shader::setUniformValue(QString name, const QVector2D& value)
  {
    SET_UNIFORM_VALUE(vec2, name, value);
  }
  
  void Shader::setUniformValue(QString name, const QVector3D& value)
  {
    SET_UNIFORM_VALUE(vec3, name, value);
  }
  
  void Shader::setUniformValue(QString name, const QVector4D& value)
  {
    SET_UNIFORM_VALUE(vec4, name, value);
  }
  
  void Shader::setUniformValue(QString name, const QColor& value)
  {
    QVector3D rgb(value.redF(), value.greenF(), value.blueF());
    SET_UNIFORM_VALUE(vec4, name, rgb);
  }
  
  void Shader::setUniformValue(QString name, const QMatrix4x4& value)
  {
    SET_UNIFORM_VALUE(mat4x4, name, value);
  }
}
