#include "State/PolygonMode.hpp"

#include "Mesh.hpp"
#include "Scene.hpp"

namespace OpenGL
{
  Scene::Scene(ShaderPointer default_shader)
  : QEnableSharedFromThis<Scene>{}
  , m_lights{}
  , m_bound_shader{}
  , m_default_shader{default_shader}
  {
  }
  
  
  Scene::~Scene()
  {
  }
  
  
  QList<LightPointer>
  Scene::allLights(const QMatrix4x4 &modelview) const
  {
    QList<LightPointer> lights;
    
    Q_FOREACH(ModelPointer m, m_models) {
      Q_FOREACH(LightPointer l, m->allLights()) {
        lights.append(
          LightPointer(
            new Light(
              Object::transform(modelview * m->transform(), l->position()),
              l->color(),
              l->attenuation()
            )
          )
        );
      }
    }
    
    return (lights);
  }
  
  
  ModelPointer
  Scene::pointIndicator(QColor x, QColor y, QColor z, float b, bool mirror)
  {
    OpenGL::MeshPointer axis(new OpenGL::Mesh);
    
    float a{mirror? -b:0};
        
    axis->drawElements(
      new OpenGL::DrawElements(
        QVector<int>()
          << axis->addVertex(Vertex(QVector3D( a,  0,  0), x))
          << axis->addVertex(Vertex(QVector3D( b,  0,  0), x))
          << axis->addVertex(Vertex(QVector3D( 0,  a,  0), y))
          << axis->addVertex(Vertex(QVector3D( 0,  b,  0), y))
          << axis->addVertex(Vertex(QVector3D( 0,  0,  a), z))
          << axis->addVertex(Vertex(QVector3D( 0,  0,  b), z)),
        GL_LINES
      )
    );
    
    return OpenGL::ModelPointer{new OpenGL::Model{axis, 0}};
  }
  
  void Scene::draw(CameraPointer camera)
  {
    const QMatrix4x4 projection(camera->projection());
    const QMatrix4x4 modelview(camera->transform());
    
    QList<LightPointer> lights(allLights(modelview));
    State::PolygonMode pmguard(GL_FRONT_AND_BACK, camera->wireframe()? GL_LINE:GL_FILL);
    
    Q_FOREACH(ModelPointer m, m_models) {
      ShaderPointer shader{bind(m->shader())};
      
      shader->setProjectionMatrix(projection);
      shader->setModelviewMatrix(modelview * m->transform());
      shader->setLights(lights);
      
      m->bind(sharedFromThis());
      m->draw(sharedFromThis());
      m->release(sharedFromThis());
    }
  }
  
  ShaderPointer Scene::defaultShader() const
  {
    return m_default_shader;
  }
  
  ShaderPointer Scene::bind(ShaderPointer shader)
  {
    if (m_bound_shader != shader) {
      m_bound_shader->release();
    }
    if (shader.isNull()) {
      m_bound_shader = m_default_shader;
    } else {
      m_bound_shader = shader;
    }
    
    m_bound_shader->bind();
    return (m_bound_shader);
  }
  
  ShaderPointer Scene::bound()
  {
    return (m_bound_shader);
  }
  
  
  void Scene::addLight(LightPointer light)
  {
    m_lights.insert(light);
  }
  
  
  void Scene::removeLight(LightPointer light)
  {
    m_lights.remove(light);
  }
  
  
  void Scene::addModel(ModelPointer model)
  {
    m_models.insert(model);
  }
  
  
  void Scene::removeModel(ModelPointer model)
  {
    m_models.remove(model);
  }
}
