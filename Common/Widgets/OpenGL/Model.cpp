#include "Mesh.hpp"
#include "Model.hpp"
#include "Scene.hpp"

#include "State/DepthMask.hpp"

namespace OpenGL
{
  Model::Model(Texturing texturing, Lighting Lighting, DepthMask depth)
  : Model{MeshPointer{new Mesh}, texturing, Lighting, depth}
  {
  }
  
  Model::Model(MeshPointer mesh, Texturing texture, Lighting light, DepthMask depth)
  : m_lights()
  , m_textures()
  , m_mesh(mesh)
  , m_shader()
  , m_enabled(true)
  , m_texturing(texture)
  , m_lighting(light)
  , m_depth(depth)
  {
  }
  
  
  Model::~Model()
  {
  }
  
  
  Model::Lights
  Model::allLights() const
  {
    return (m_lights);
  }
  
  
  void
  Model::addLight(LightPointer light)
  {
    m_lights.insert(light);
  }
  
  
  void
  Model::removeLight(LightPointer light)
  {
    m_lights.remove(light);
  }
  
  
  void
  Model::setEnabled(bool enabled)
  {
    m_enabled = enabled;
  }
  
  
  bool
  Model::enabled() const
  {
    return m_enabled;
  }
  
  
  void Model::setLighting(bool enabled)
  {
    m_lighting = Lighting{enabled};
  }
  
  bool Model::lighting() const
  {
    return m_lighting;
  }
    
  void Model::setTexturing(bool enabled)
  {
    m_texturing = Texturing{enabled};
  }
  
  bool Model::texturing() const
  {
    return m_texturing;
  }
    
  void Model::setDepthMask(bool enabled)
  {
    m_depth = DepthMask{enabled};
  }
  
  bool Model::depthMask() const
  {
    return m_depth;
  }
  
  
  void 
  Model::setMesh(MeshPointer mesh)
  {
    m_mesh = mesh;
  }
  
  
  MeshPointer
  Model::mesh() const
  {
    return (m_mesh);
  }
  
  
  void
  Model::setShader(ShaderPointer shader)
  {
    m_shader = shader;
  }
  
  
  ShaderPointer
  Model::shader() const
  {
    return (m_shader);
  }
  
  
  void
  Model::setTexture(TexturePurpose purpose, QImage image)
  {
    m_textures[purpose] = TexturePointer(new Texture(image));
  }
  
  
  void
  Model::bind(ScenePointer ctx)
  {
    ShaderPointer shader{ctx->bind(m_shader)};
    
    Q_FOREACH(TexturePurpose purpose, m_textures.keys()) {
      Texture::Unit unit(purpose);
      shader->setTextureUnitEnabled(unit, m_textures[purpose]->bind(unit));
    }
    
    m_mesh->bind(shader);
  }
  
  
  void
  Model::release(ScenePointer ctx)
  {
    ShaderPointer shader{ctx->bound()};
    m_mesh->release(shader);
    
    Q_FOREACH(TexturePurpose purpose, m_textures.keys()) {
      Texture::Unit unit(purpose);
      shader->setTextureUnitEnabled(unit, false);
      m_textures[purpose]->release(unit);
    }
  }
  
  
  void
  Model::draw(ScenePointer ctx)
  {
    if (m_enabled) {
      draw(ctx,m_texturing, m_lighting, m_depth);
    }
  }
  
  void
  Model::draw(ScenePointer ctx, Texturing texturing, Lighting lighting, DepthMask depth)
  {
    if (m_enabled) {
      ShaderPointer shader{ctx->bound()};
    
      shader->setTexturingEnabled(texturing);
      shader->setLightingEnabled(lighting);
      
      State::DepthMask d(depth);
      m_mesh->draw(shader);
    }
  }
}
