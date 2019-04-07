#include "Mesh.hpp"
#include "Model.hpp"
#include "Scene.hpp"

#include "State/DepthMask.hpp"

namespace OpenGL
{
  Model::Model(int rendering_attributes)
  : Model(MeshPointer(new Mesh), rendering_attributes)
  {
  }
  
  Model::Model(MeshPointer mesh, int flags)
  : m_lights()
  , m_textures()
  , m_mesh(mesh)
  , m_shader()
  , m_flags(flags)
  {
  }
  
  
  Model::~Model()
  {
  }
  
  
  QSet<LightPointer>
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
  
  
  int Model::flags() const
  {
    return (m_flags);
  }
  
  
  void Model::setFlags(int flags)
  {
    m_flags = flags;
  }
  
  
  void
  Model::setTexture(Texture::Unit unit, QImage image)
  {
    m_textures[unit] = TexturePointer(new Texture(image));
  }
  
  
  void
  Model::bind(ScenePointer ctx)
  {
    ShaderPointer shader{ctx->bind(m_shader)};
    
    Q_FOREACH(Texture::Unit unit, m_textures.keys()) {
      shader->setTextureUnitEnabled(unit, m_textures[unit]->bind(unit));
    }
    
    m_mesh->bind(shader);
  }
  
  
  void
  Model::release(ScenePointer ctx)
  {
    ShaderPointer shader{ctx->bound()};
    m_mesh->release(shader);
    
    Q_FOREACH(Texture::Unit unit, m_textures.keys()) {
      shader->setTextureUnitEnabled(unit, false);
      m_textures[unit]->release(unit);
    }
  }
  
  
  void
  Model::draw(ScenePointer ctx)
  {
    draw(ctx, m_flags);
  }
  
  void
  Model::draw(ScenePointer ctx, int flags)
  {
    ShaderPointer shader{ctx->bound()};
    
    shader->setTexturingEnabled(flags & Texturing);
    shader->setLightingEnabled(flags & Lighting);
    
    State::DepthMask d((flags & DepthMasked) == 0);
    m_mesh->draw(shader);
  }
}
