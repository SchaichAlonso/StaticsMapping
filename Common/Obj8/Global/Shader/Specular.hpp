#pragma once

#include <QtGlobal>

#include <Obj8/AbstractShader.hpp>

namespace Obj8
{
  namespace Global
  {
    namespace Shader
    {
      struct Specular : AbstractShader
      {
        Specular ();
        Specular (StringRef, Parser::LexerContext *);
        virtual ~Specular ();
        
        virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
        virtual String        name () const Q_DECL_OVERRIDE;
      };
    }
  }
}
