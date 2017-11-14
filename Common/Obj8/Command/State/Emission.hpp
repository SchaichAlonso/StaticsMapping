#pragma once

#include <QtGlobal>

#include <Obj8/AbstractMaterial.hpp>

namespace Obj8
{
  namespace Command
  {
    namespace State
    {
      struct Emission : AbstractMaterial
      {
        Emission ();
        Emission (StringRef, Parser::LexerContext *);
        virtual ~Emission ();
        
        virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
        virtual String        name () const Q_DECL_OVERRIDE;
      };
    }
  }
}
