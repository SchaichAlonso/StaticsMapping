#pragma once

#include "Parameter/Word.hpp"
#include "Parser/Word.hpp"

#include "Record.hpp"
#include "RecordSequence.hpp"

namespace Obj8
{
  struct ConditionalBranch
  {
    ConditionalBranch (StringRef, Parser::LexerContext *, bool);
    
    String condition () const;
    bool   conditional () const;
    bool   negated () const;
    
    bool   accept (AbstractVisitor *, bool);
    String toString () const;
    
    Parameter::Word m_negation;
    Parameter::Word m_condition;
    RecordSequence m_branch;
    
    bool m_conditional;
    bool m_negated;
  };
  
  struct Conditional : Record
  {
    Conditional ();
    Conditional (StringRef, Parser::LexerContext *);
    virtual ~Conditional ();
    
    virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
    virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
    virtual String        name () const Q_DECL_OVERRIDE;
    virtual String        toString () const Q_DECL_OVERRIDE;
    
    
  protected:
    QList<Parser::Word>             m_keywords;
    QList<ConditionalBranchPointer> m_branches;
  };
}
