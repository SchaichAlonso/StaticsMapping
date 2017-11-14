#pragma once

#include <QtGlobal>

#include <Obj8/AbstractMaterial.hpp>

namespace Obj8
{
  namespace Command
  {
    namespace State
    {
      struct Specular : AbstractMaterial
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
