#pragma once

#include <QtGlobal>

#include "Obj8.hpp"
#include "Record.hpp"

namespace Obj8
{
  struct Comment : Record
  {
    Comment ();
    Comment (StringRef, Parser::LexerContext *, int =0);
    virtual ~Comment ();
    
    static bool qualified (Parser::Word);
    
    virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
    virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
    virtual String        name () const Q_DECL_OVERRIDE;
  };
}
