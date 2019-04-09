#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QSet>

#include <QtGui/QMatrix4x4>

#include "Camera/Camera.hpp"
#include "Light.hpp"
#include "Model.hpp"
#include "Shader.hpp"

namespace OpenGL
{
  struct Scene : QEnableSharedFromThis<Scene>
  {
    typedef QSet<LightPointer> Lights;
    typedef QSet<ModelPointer> Models;
    
  public:
    Scene(ShaderPointer default_shader);
   ~Scene();
    
    void addLight(LightPointer light);
    void removeLight(LightPointer light);
    
    ModelWeakPointer insertModel(int rendering_attributes=Model::Lighting|Model::Texturing);
    void removeModel(ModelPointer model);
    
    void draw(CameraPointer camera);
    
    ShaderPointer defaultShader() const;
    ShaderPointer bind(ShaderPointer shader);
    ShaderPointer bound();
    
    ModelWeakPointer insertPositionIndicator(QColor x, QColor y, QColor z, float axis_length=1, bool mirror=true);
    ModelWeakPointer insertGrid(QVector3D u, QVector3D v, int repetitions, bool surface);
    
  protected:
    RawImage lightTexture(Lights lights);
    Lights allLights(QMatrix4x4 modelview) const;
    Models insertLightIndicators(QMatrix4x4 modelview, Lights lights);
    
  protected:
    float m_roll, m_pitch, m_yaw;
    
    QColor m_ambient;
    Lights m_lights;
    Models m_models;
    
    ShaderPointer m_bound_shader;
    ShaderPointer m_default_shader;
  };
  
  typedef QSharedPointer<Scene> ScenePointer;
  typedef QWeakPointer<Scene> SceneWeakPointer;
}
