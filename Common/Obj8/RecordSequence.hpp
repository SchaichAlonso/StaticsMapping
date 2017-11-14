#pragma once

#include <QLinkedList>

#include "Record.hpp"


namespace Obj8
{
  struct RecordSequence
  {
    enum EmptyWordPolicy {
      AcceptEmpty,
      RejectEmpty
    };
    
    RecordSequence ();
    RecordSequence (StringRef, Parser::LexerContext *, EmptyWordPolicy=AcceptEmpty);
    virtual ~RecordSequence ();
  
    String toString () const;

    virtual void accept (AbstractVisitor *, bool);
    
  protected:
    QLinkedList<RecordPointer> m_contents;
  };
}
