#pragma once

#include <QtCore/QVector>
#include <QtCore/QSharedPointer>

#include "Shader.hpp"

namespace OpenGL
{
  typedef QVector<int> IndexArray;
  
  struct DrawElements
  {
    DrawElements(IndexArray indices=IndexArray(), GLenum mode=GLenum());
    virtual ~DrawElements();
    
    virtual void draw(OpenGL::ShaderPointer shader);
    
  protected:
    IndexArray m_indices;
    GLenum m_mode;
  };
  
  typedef QSharedPointer<DrawElements> DrawElementsPointer;
}
