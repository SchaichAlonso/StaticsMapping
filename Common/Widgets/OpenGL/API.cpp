#include <QtGui/QOpenGLContext>

#include "API.hpp"

namespace OpenGL
{
  API *api()
  {
    return (QOpenGLContext::currentContext()->versionFunctions<API>());
  }
}
