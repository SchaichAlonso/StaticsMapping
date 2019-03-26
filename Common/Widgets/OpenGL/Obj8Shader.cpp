#include <Common/DataPath.hpp>

#include "Obj8Shader.hpp"

namespace OpenGL
{
  Obj8Shader::Obj8Shader()
  : Shader(
      DataPath::existingPath("obj8-vert.glsl"),
      DataPath::existingPath("obj8-frag.glsl")
    )
  {
  }
  
  
  Obj8Shader::~Obj8Shader()
  {
  }
}
