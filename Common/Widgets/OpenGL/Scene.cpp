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
  
  
  RawImage Scene::lightTexture(Lights lights)
  {
    RawImage r(QSize(lights.size(), 8));
    int x=0;
    Q_FOREACH(LightPointer light, lights) {
      r.setPixel(x, 0, light->position());
      r.setPixel(x, 1, light->color());
      r.setPixel(x, 2, light->attenuation());
      r.setPixel(x, 3, QVector2D(light->range(), light->rangeExp()));
      r.setPixel(x, 4, light->spotDirection());
      r.setPixel(x, 5, QVector2D(light->spotCutoffAngle(), light->spotExp()));
      
      x++;
    }
    
    return (r);
  }
  
  
  Scene::Lights
  Scene::allLights(QMatrix4x4 modelview) const
  {
    Lights lights;
    
    Q_FOREACH(ModelPointer m, m_models) {
      Q_FOREACH(LightPointer l, m->allLights()) {
        lights.insert(LightPointer{new Light{modelview * m->transform(), *l}});
      }
    }
    
    return (lights);
  }
  
  
  ModelWeakPointer Scene::insertPositionIndicator(QColor x, QColor y, QColor z, float b, bool mirror)
  {
    ModelPointer model{insertModel(0)};
    MeshPointer axis{model->mesh()};
    
    float a{mirror? -b:0};
        
    axis->drawElements(
      new DrawElements(
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
    
    return (model);
  }
  
  ModelWeakPointer Scene::insertGrid(QVector3D u, QVector3D v, int repetitions, bool surface)
  {
    ModelPointer model{insertModel(Model::Lighting)};
    MeshPointer mesh{model->mesh()};
    
    QVector3D normal(QVector3D::normal(u, v));
    
    IndexArray indices{};
    for (int i=-repetitions; i<repetitions; ++i) {
      QVector4D color(1, 1, 1, 1);
      if (i%100 != 0)
        color.setW(0.75);
      if (i%10 != 0)
        color.setW(0.5);
        
      indices << mesh->addVertex(Vertex(i*u + repetitions*v, color, normal, QVector2D(i, repetitions)));
      indices << mesh->addVertex(Vertex(i*u - repetitions*v, color, normal, QVector2D(i,-repetitions)));
      
      indices << mesh->addVertex(Vertex(i*v + repetitions*u, color, normal, QVector2D( repetitions, i)));
      indices << mesh->addVertex(Vertex(i*v - repetitions*u, color, normal, QVector2D(-repetitions, i)));
    }
    
    mesh->drawElements(DrawElementsPointer(new DrawElements(indices, GL_LINES)));
    
    if (surface) {
      QVector4D floor(1, 1, 1, 0.5);
      QVector3D offset(0.01*normal);
      mesh->drawElements(
        DrawElementsPointer(
          new DrawElements(
            IndexArray()
              << mesh->addVertex(Vertex(offset - repetitions*u - repetitions*v, floor, normal, QVector2D(-repetitions, -repetitions)))
              << mesh->addVertex(Vertex(offset - repetitions*u + repetitions*v, floor, normal, QVector2D(-repetitions,  repetitions)))
              << mesh->addVertex(Vertex(offset + repetitions*u + repetitions*v, floor, normal, QVector2D( repetitions,  repetitions)))
              << mesh->addVertex(Vertex(offset - repetitions*u - repetitions*v, floor, normal, QVector2D(-repetitions, -repetitions)))
              << mesh->addVertex(Vertex(offset + repetitions*u + repetitions*v, floor, normal, QVector2D( repetitions,  repetitions)))
              << mesh->addVertex(Vertex(offset + repetitions*u - repetitions*v, floor, normal, QVector2D( repetitions, -repetitions))),
            GL_TRIANGLES
          )
        )
      );
    }
    
    return (model);
  }
  
  void Scene::draw(CameraPointer camera)
  {
    const QMatrix4x4 projection(camera->projection());
    const QMatrix4x4 modelview(camera->transform());
    
    Lights lights(allLights(modelview));
    Models temporaries(insertLightIndicators(modelview, lights));
    State::PolygonMode pmguard(GL_FRONT_AND_BACK, camera->wireframe()? GL_LINE:GL_FILL);
    
    TexturePointer light_tex(new Texture(lightTexture(lights)));
    
    Q_FOREACH(ModelPointer m, m_models) {
      ShaderPointer shader{bind(m->shader())};
      
      shader->setProjectionMatrix(projection);
      shader->setModelviewMatrix(modelview * m->transform());
      shader->setLights(lights.toList());
      shader->setTextureUnitEnabled(7, light_tex->bind(7));
      
      m->bind(sharedFromThis());
      if (camera->wireframe()) {
        m->draw(sharedFromThis(), 0);
      } else {
        m->draw(sharedFromThis());
      }
      
      m->release(sharedFromThis());
      
      shader->setTextureUnitEnabled(7, false);
    }
    
    Q_FOREACH(ModelPointer temp, temporaries) {
      removeModel(temp);
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
  
  
  ModelWeakPointer Scene::insertModel(int rendering_attributes)
  {
    return (*m_models.insert(ModelPointer(new Model(rendering_attributes))));
  }
  
  
  void Scene::addModel(ModelPointer model)
  {
    m_models.insert(model);
  }
  
  
  void Scene::removeModel(ModelPointer model)
  {
    m_models.remove(model);
  }
  
  
  Scene::Models Scene::insertLightIndicators(QMatrix4x4 modelview, Lights lights)
  {
    Models retval;
    QMatrix4x4 inverse(modelview.inverted());
    
    Q_FOREACH(LightPointer light, lights) {
      QColor color{light->color()};
      ModelPointer indicator(insertPositionIndicator(color, color, color));
      indicator->setPosition(inverse * light->position());
      retval.insert(indicator);
    }
    return (retval);
  }

}
