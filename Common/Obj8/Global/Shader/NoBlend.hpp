#pragma once

#include <QtGlobal>

#include <Obj8/AbstractShader.hpp>

namespace Obj8
{
  namespace Global
  {
    namespace Shader
    {
      struct NoBlend : AbstractShader
      {
        NoBlend ();
        NoBlend (StringRef, Parser::LexerContext *);
        virtual ~NoBlend ();
        
        virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
        virtual String        name () const Q_DECL_OVERRIDE;
      };
    }
  }
}
