#pragma once

#include <QtGlobal>

#include <Obj8/AbstractShader.hpp>

namespace Obj8
{
  namespace Global
  {
    namespace Shader
    {
      struct BumpLevel : AbstractShader
      {
        BumpLevel ();
        BumpLevel (StringRef, Parser::LexerContext *);
        virtual ~BumpLevel ();
        
        virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
        virtual String        name () const Q_DECL_OVERRIDE;
      };
    }
  }
}
