#pragma once

#include <QtGlobal>

#include <Obj8/AbstractMaterial.hpp>

namespace Obj8
{
  namespace Command
  {
    namespace State
    {
      struct Ambient : AbstractMaterial
      {
        Ambient ();
        Ambient (StringRef, Parser::LexerContext *);
        virtual ~Ambient ();
        
        virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
        virtual String        name () const Q_DECL_OVERRIDE;
      };
    }
  }
}
