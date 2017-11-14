#pragma once

#include <QtGlobal>

#include <Obj8/AbstractShader.hpp>

namespace Obj8
{
  namespace Global
  {
    namespace Shader
    {
      struct TwoSided : AbstractShader
      {
        TwoSided ();
        TwoSided (StringRef, Parser::LexerContext *);
        virtual ~TwoSided ();
        
        virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
        virtual String        name () const Q_DECL_OVERRIDE;
      };
    }
  }
}
