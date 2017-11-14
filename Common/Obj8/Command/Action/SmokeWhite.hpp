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
      struct SmokeWhite : AbstractSmoke
      {
        SmokeWhite ();
        SmokeWhite (StringRef, Parser::LexerContext *);
        virtual ~SmokeWhite ();
        
        virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
        virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
        virtual String        name () const Q_DECL_OVERRIDE;
      };
    }
  }
}
