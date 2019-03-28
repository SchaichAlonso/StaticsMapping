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
  
  void Scene::draw(CameraPointer camera)
  {
    const QMatrix4x4 projection(camera->projection());
    const QMatrix4x4 modelview(camera->transform());
    
    QList<LightPointer> lights(allLights(modelview));
    
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
