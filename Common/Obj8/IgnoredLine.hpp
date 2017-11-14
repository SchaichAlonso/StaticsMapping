#pragma once

#include <QtGlobal>

#include "Obj8.hpp"
#include "Record.hpp"

namespace Obj8
{
  struct IgnoredLine : Record
  {
    IgnoredLine ();
    IgnoredLine (StringRef, Parser::LexerContext *);
    virtual ~IgnoredLine ();
    
    virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
    virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
    virtual String        name () const Q_DECL_OVERRIDE;
  };
}
