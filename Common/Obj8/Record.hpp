#pragma once

#include "Parser/LexerContext.hpp"
#include "Parser/Word.hpp"

#include "AbstractVisitor.hpp"
#include "Obj8.hpp"

namespace Obj8
{
  struct Record
  {
    Record ();
    Record (StringRef, Parser::LexerContext * =Q_NULLPTR, int =Parser::Word::PrecedingLinebreakRequired);
    virtual ~Record();
    
    virtual void          accept (AbstractVisitor *, bool) = 0;
    virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const = 0;
    virtual String        name () const = 0;
    virtual String        toString () const;
    
  protected:
    Parser::Word m_identifier;
  };
}
