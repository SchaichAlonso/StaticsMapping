#include "Shader.hpp"

namespace OpenGL
{
  Shader::Shader(QString vertex, QString fragment)
  : m_impl()
  , m_vert_shader_filename(vertex)
  , m_frag_shader_filename(fragment)
  {
  }
  
  Shader::~Shader()
  {
  }
  
  
  void Shader::setTextureUnitEnabled(int unit, bool enabled)
  {
    setUniformValue(QString("texture_unit[%1]").arg(unit), unit);
    setUniformValue(QString("texture_unit_enabled[%1]").arg(unit), enabled);
  }
  
  
  void Shader::setTexturingEnabled(bool enable)
  {
    setUniformValue("texturing", enable);
  }
  
  
  void Shader::setLightingEnabled(bool enable)
  {
    setUniformValue("light_enabled", enable);
  }
  
  
  void Shader::setLights(QList<LightPointer> lights)
  {
    int count(0);
    Q_FOREACH(LightPointer l, lights) {
      setUniformValue(QString("lights[%1].position").arg(count), l->position());
      setUniformValue(QString("lights[%1].color").arg(count), l->color());
      setUniformValue(QString("lights[%1].attenuation").arg(count), l->attenuation());
      setUniformValue(QString("lights[%1].spotDirection").arg(count), l->spotDirection());
      setUniformValue(QString("lights[%1].range").arg(count), l->range());
      setUniformValue(QString("lights[%1].rangeExp").arg(count), l->rangeExp());
      setUniformValue(QString("lights[%1].spotCutoffAngle").arg(count), l->spotCutoffAngle());
      setUniformValue(QString("lights[%1].spotExp").arg(count), l->spotExp());
      if(++count >= 64) {
        qCritical("Light count too high!");
        break;
      }
    }
    setUniformValue("light_count", count);
  }
  
  void Shader::setModelviewMatrix(const QMatrix4x4 &value)
  {
    setUniformValue("modelview", value);
  }
  
  void Shader::setProjectionMatrix(const QMatrix4x4 &value)
  {
    setUniformValue("projection", value);
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
      qCritical("Failed to create Shader %s", qUtf8Printable(shader->log()));
      shader.reset();
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
  
  void Shader::bind()
  {
    if (m_impl.isNull()) {
      m_impl = createShader();
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
