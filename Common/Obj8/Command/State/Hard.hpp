#pragma once

#include <QtGlobal>

#include <Obj8/AbstractHard.hpp>

namespace Obj8
{
  namespace Command
  {
    namespace State
    {
      struct Hard : AbstractHard
      {
        Hard ();
        Hard (StringRef, Parser::LexerContext *);
        virtual ~Hard ();
        
        virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
        virtual String        name () const Q_DECL_OVERRIDE;
      };
    }
  }
}
