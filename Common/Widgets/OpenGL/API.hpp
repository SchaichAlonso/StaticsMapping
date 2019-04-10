#pragma once

#include <QtGui/QOpenGLFunctions_3_3_Core>

namespace OpenGL
{
  typedef QOpenGLFunctions_3_3_Core API;
  
  API *api();
}
