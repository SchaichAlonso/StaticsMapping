#pragma once

#include <QtGlobal>

#include <Obj8/Obj8.hpp>
#include <Obj8/AbstractSmoke.hpp>

namespace Obj8
{
  namespace Command
  {
    namespace Action
    {
      struct SmokeBlack : AbstractSmoke
      {
        SmokeBlack ();
        SmokeBlack (StringRef, Parser::LexerContext *);
        virtual ~SmokeBlack ();
        
        virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
        virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
        virtual String        name () const Q_DECL_OVERRIDE;
      };
    }
  }
}
