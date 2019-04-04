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
    Scene(ShaderPointer default_shader);
   ~Scene();
    
    void addLight(LightPointer light);
    void removeLight(LightPointer light);
    
    void addModel(ModelPointer model);
    void removeModel(ModelPointer model);
    
    void draw(CameraPointer camera);
    
    ShaderPointer defaultShader() const;
    ShaderPointer bind(ShaderPointer shader);
    ShaderPointer bound();
    
    QList<LightPointer> allLights(const QMatrix4x4 &modelview) const;
    QList<ModelPointer> allModels(const QMatrix4x4 &modelview, QList<LightPointer> lights) const;
    
    static ModelPointer pointIndicator(QColor x, QColor y, QColor z, float axis_length=1, bool mirror=true);
    
  protected:
    float m_roll, m_pitch, m_yaw;
    
    typedef QSet<LightPointer> Lights;
    Lights m_lights;
    QSet<ModelPointer> m_models;
    
    ShaderPointer m_bound_shader;
    ShaderPointer m_default_shader;
  };
  
  typedef QSharedPointer<Scene> ScenePointer;
}
