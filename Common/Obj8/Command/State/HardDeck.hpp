#pragma once

#include <QtGlobal>

#include <Obj8/AbstractHard.hpp>

namespace Obj8
{
  namespace Command
  {
    namespace State
    {
      struct HardDeck : AbstractHard
      {
        HardDeck ();
        HardDeck (StringRef, Parser::LexerContext *);
        virtual ~HardDeck ();
        
        virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
        virtual String        name () const Q_DECL_OVERRIDE;
      };
    }
  }
}
 
