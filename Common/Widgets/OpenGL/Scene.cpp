#include "State/PolygonMode.hpp"

#include "Mesh.hpp"
#include "Scene.hpp"

namespace OpenGL
{
  Scene::Scene(ShaderPointer default_shader)
  : QEnableSharedFromThis<Scene>{}
  , m_ambient{128, 128, 128}
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
    RawImage r(QSize(8, lights.size()));
    int y=0;
    Q_FOREACH(LightPointer light, lights) {
      r.setPixel(0, y, light->position());
      r.setPixel(1, y, light->color());
      r.setPixel(2, y, light->attenuation());
      r.setPixel(3, y, QVector2D(light->range(), light->rangeExp()));
      r.setPixel(4, y, light->spotDirection());
      r.setPixel(5, y, QVector2D(light->spotCutoffAngle(), light->spotExp()));
      
      y++;
    }
    
    return (r);
  }
  
  
  Scene::Lights
  Scene::allLights(QMatrix4x4 modelview) const
  {
    Lights lights;
    
    Q_FOREACH(ModelPointer m, m_models) {
      if (m->enabled()) {
        Q_FOREACH(LightPointer l, m->allLights()) {
          lights.insert(LightPointer{new Light{modelview * m->transform(), *l}});
        }
      }
    }
    
    return (lights);
  }
  
  
  ModelPointer Scene::positionIndicator(QColor x, QColor y, QColor z, float b, bool mirror)
  {
    ModelPointer model{new Model()};
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
  
  
  ModelWeakPointer Scene::insertPositionIndicator(QColor x, QColor y, QColor z, float b, bool mirror)
  {
    return insertModel(positionIndicator(x, y, z, b, mirror));
  }
  
  ModelPointer Scene::plane(QVector3D u, QVector3D v, int repetitions)
  {
    ModelPointer model{new Model};
    MeshPointer mesh{model->mesh()};
    
    QVector3D normal(QVector3D::normal(u, v));
    
    QVector4D floor(1, 1, 1, 1);
    QVector3D offset(-0.01*normal);
    
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
    
    return (model);
  }
  
  ModelPointer Scene::grid(QVector3D u, QVector3D v, int repetitions)
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
    
    return (model);
  }
  
  void Scene::draw(CameraPointer camera)
  {
    const QMatrix4x4 projection(camera->projection());
    const QMatrix4x4 modelview(camera->transform());
    
    Lights lights(allLights(modelview));
    Models temporaries(lightIndicators(modelview, lights));
    State::PolygonMode pmguard(GL_FRONT_AND_BACK, camera->wireframe()? GL_LINE:GL_FILL);
    
    TexturePointer light_tex(new Texture(lightTexture(lights)));
    
    Q_FOREACH(ModelPointer m, m_models + temporaries) {
      if (m->enabled()) {
        ShaderPointer shader{bind(m->shader())};
      
        shader->setProjectionMatrix(projection);
        shader->setModelviewMatrix(modelview * m->transform());
        shader->setLights(lights.toList());
        shader->setAmbientColor(m_ambient);
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
  
  
  ModelWeakPointer Scene::insertModel(ModelPointer model)
  {
    return (*m_models.insert(model));
  }
  
  
  ModelWeakPointer Scene::insertModel(int rendering_attributes)
  {
    return insertModel(ModelPointer(new Model(rendering_attributes)));
  }
  
  
  void Scene::removeModel(ModelPointer model)
  {
    m_models.remove(model);
  }
  
  
  Scene::Models Scene::lightIndicators(QMatrix4x4 modelview, Lights lights)
  {
    Models retval;
    QMatrix4x4 inverse(modelview.inverted());
    
    Q_FOREACH(LightPointer light, lights) {
      QColor color{light->color()};
      ModelPointer indicator(positionIndicator(color, color, color));
      indicator->setPosition(inverse * light->position());
      retval.insert(indicator);
    }
    return (retval);
  }

}
