#pragma once

#include "DrawElements.hpp"

namespace OpenGL {
  
  struct Obj8RenderAttributes
  {
    Obj8RenderAttributes(int lod_near, int lod_far, bool is_draped);
    
    int  lod_near;
    int  lod_far;
    bool is_draped;
  };
  
  struct DrawElementsObj8 : DrawElements
  {
    DrawElementsObj8(IndexArray indices, bool is_line, Obj8RenderAttributes attribs);
    virtual ~DrawElementsObj8();
    
    virtual void draw(OpenGL::ShaderPointer shader) Q_DECL_OVERRIDE;
    
  protected:
    Obj8RenderAttributes m_attribs;
  };
}
