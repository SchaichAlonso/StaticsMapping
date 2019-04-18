#pragma once

#include <QtCore/QSet>
#include <QtCore/QSharedPointer>
#include <QtWidgets/QOpenGLWidget>

#include "Light.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Object.hpp"

namespace OpenGL
{
  struct Scene;
  typedef QSharedPointer<Scene> ScenePointer;
  
  struct Model : Object
  {
    enum RenderingAttributes {
      Texturing   = 1 << 0,
      Lighting    = 1 << 1,
      DepthMasked = 1 << 2
    };
    
    typedef QSet<LightPointer> Lights;
    
    Model(int flags=Texturing|Lighting);
    Model(MeshPointer mesh, int flags=Texturing|Lighting);
   ~Model();
    
    Lights allLights() const;
    void addLight(LightPointer light);
    void removeLight(LightPointer light);
    
    void setEnabled(bool enabled);
    bool enabled() const;
    
    void setMesh(MeshPointer mesh);
    MeshPointer mesh() const;
    
    void setShader(ShaderPointer shader);
    ShaderPointer shader() const;
    
    int flags() const;
    void setFlags(int flags);
    
    void setTexture(Texture::Unit unit, QImage image);
    
    void bind(ScenePointer ctx);
    void release(ScenePointer ctx);
    void draw(ScenePointer ctx);
    void draw(ScenePointer ctx, int flags);
    
  protected:
    Lights m_lights;
    QMap<Texture::Unit, TexturePointer> m_textures;
    MeshPointer m_mesh;
    ShaderPointer m_shader;
    int m_flags;
    bool m_enabled;
  };
  
  typedef QSharedPointer<Model> ModelPointer;
  typedef QWeakPointer<Model> ModelWeakPointer;
}
