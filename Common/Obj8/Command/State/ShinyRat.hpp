#pragma once

#include <QtGlobal>

#include <Obj8/AbstractSpecular.hpp>

namespace Obj8
{
  namespace Command
  {
    namespace State
    {
      struct ShinyRat : AbstractSpecular
      {
        ShinyRat ();
        ShinyRat (StringRef, Parser::LexerContext *);
        virtual ~ShinyRat ();
        
        virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
        virtual String        name () const Q_DECL_OVERRIDE;
      };
    }
  }
}
 
