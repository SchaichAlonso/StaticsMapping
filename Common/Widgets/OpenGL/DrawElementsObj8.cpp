#include "DrawElementsObj8.hpp"

namespace OpenGL 
{
  Obj8RenderAttributes::Obj8RenderAttributes(int lod_near, int lod_far, bool draped)
  : lod_near(lod_near)
  , lod_far(lod_far)
  , is_draped(draped)
  {
  }
  
  
  
  
  DrawElementsObj8::DrawElementsObj8(IndexArray indices, bool is_line, Obj8RenderAttributes attribs)
  : DrawElements(
      indices,
      is_line? GL_LINES : GL_TRIANGLES
    ),
    m_attribs(attribs)
  {
  }
  
  
  DrawElementsObj8::~DrawElementsObj8()
  {
  }
  
  
  void
  DrawElementsObj8::draw(OpenGL::ShaderPointer shader)
  {
    if(m_attribs.lod_near != 0) {
      return;
    }
    
    shader->setUniformValue("texture_draped_enabled", m_attribs.is_draped);
    
    DrawElements::draw(shader);
  }
}
