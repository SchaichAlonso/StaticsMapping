#pragma once

#include <QtCore/QSet>
#include <QtCore/QSharedPointer>
#include <QtWidgets/QOpenGLWidget>

#include "Camera/Camera.hpp"
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
  private:
    template<typename ImplName, typename Type> struct NamedPod {
      NamedPod(const Type &value=Type()) : m_value{value}
      {
      }
      
      NamedPod(const ImplName &other) : m_value{other.m_value}
      {
      }
      
      operator Type() const
      {
        return m_value;
      }
      
      ImplName &operator=(const Type &other)
      {
        return *this = ImplName(other);
      }
      
      ImplName &operator=(const ImplName &other)
      {
        if (this != &other) {
          m_value = other.m_value;
        }
        return *this;
      }
      
    protected:
      Type m_value;
    };
  public:
    struct Texturing : NamedPod<Texturing,bool>
    {
      explicit Texturing(const bool &value) : NamedPod{value} {}
      Texturing(const Texturing &value) : NamedPod{value} {}
    };
    struct Lighting : NamedPod<Lighting,bool>
    {
      explicit Lighting(const bool &value) : NamedPod{value} {}
      Lighting(const Lighting &value) : NamedPod{value} {}
    };
    struct DepthMask : NamedPod<Texturing,bool>
    {
      explicit DepthMask(const bool &value) : NamedPod{value} {}
      DepthMask(const DepthMask &value) : NamedPod{value} {}
    };
    
    typedef QSet<LightPointer> Lights;
    
  public:
    enum TexturePurpose {
      TextureRegular = 0,
      TextureNormal = -1,
      TextureLit = 2,
      TextureDraped = 1
    };
    
  public:
    Model(Texturing texturing=Texturing{true}, Lighting lighting=Lighting{true}, DepthMask depth=DepthMask{true});
    Model(MeshPointer mesh, Texturing texturing=Texturing{true}, Lighting lighting=Lighting{true}, DepthMask depth=DepthMask{true});
   ~Model();
    
    Lights allLights() const;
    void addLight(LightPointer light);
    void removeLight(LightPointer light);
    
    void setEnabled(bool enabled);
    bool enabled() const;
    
    void setLighting(bool enabled);
    bool lighting() const;
    
    void setTexturing(bool enabled);
    bool texturing() const;
    
    void setDepthMask(bool enabled);
    bool depthMask() const;
    
    void setMesh(MeshPointer mesh);
    MeshPointer mesh() const;
    
    void setShader(ShaderPointer shader);
    ShaderPointer shader() const;
    
    int flags() const;
    void setFlags(int flags);
    
    void setTexture(TexturePurpose unit, QImage image);
    
    void bind(ScenePointer ctx);
    void release(ScenePointer ctx);
    void draw(ScenePointer ctx);
    void draw(ScenePointer ctx, Texturing texturing, Lighting lighting, DepthMask depth);
    
  protected:
    Lights m_lights;
    QMap<TexturePurpose, TexturePointer> m_textures;
    MeshPointer m_mesh;
    ShaderPointer m_shader;
    bool m_enabled;
    Texturing m_texturing;
    Lighting m_lighting;
    DepthMask m_depth;
  };
  
  typedef QSharedPointer<Model> ModelPointer;
  typedef QWeakPointer<Model> ModelWeakPointer;
}
