#include "Parser/SyntaxError.hpp"
#include "Conditional.hpp"



Obj8::ConditionalBranch::ConditionalBranch (StringRef input,
                                            Parser::LexerContext *ctx,
                                            bool conditional)
  : m_negation ()
  , m_condition ()
  , m_branch ()
  , m_conditional (conditional)
  , m_negated (false)
{
  if (conditional) {
    Parameter::Word w;
    
    try {
      w = Parameter::Word::peek (input, ctx);
      m_negated = (w.value() == "NOT");
    } catch (const Parser::SyntaxError &) {
      m_negated = false;
    }

    if (m_negated) {
      m_negation = Parameter::Word::read (input, ctx);
    }
    m_condition = Parameter::Word::read (input, ctx);
  }
  m_branch = RecordSequence (input, ctx, RecordSequence::RejectEmpty);
}



Obj8::String
Obj8::ConditionalBranch::condition () const
{
  return (m_condition.value ());
}



bool
Obj8::ConditionalBranch::conditional () const
{
  return (m_conditional);
}



bool
Obj8::ConditionalBranch::negated () const
{
  return (m_negated);
}



bool
Obj8::ConditionalBranch::accept (AbstractVisitor *visitor, bool always)
{
  bool satisfied;
  
  if (m_conditional and not always) {
    StringRef c = condition ();
    satisfied = visitor->satisfied (c);
    satisfied = m_negated? not satisfied : satisfied;
  } else {
    satisfied = true;
  }
  
  if (satisfied) {
    m_branch.accept (visitor, always);
  }
  
  return (satisfied);
}



Obj8::String
Obj8::ConditionalBranch::toString () const
{
  String retval;
  
  retval += m_negation.toString ();
  retval += m_condition.toString ();
  retval += m_branch.toString ();
  
  return (retval);
}



Obj8::Conditional::Conditional ()
  : Record ()
{
}



Obj8::Conditional::Conditional (StringRef input, Parser::LexerContext *ctx)
  : Record (input, ctx)
{
  Parser::Word keyword;
  Parameter::Word lookahead;
  bool subcondition;
  
  keyword = m_identifier;
  if (keyword.value() != "IF") {
    throw (Parser::SyntaxError (ctx));
  }
  
  m_keywords.append (keyword);
  m_branches.append (
    ConditionalBranchPointer(
      new ConditionalBranch (input, ctx, true)
    )
  );

  for (;;) {
    keyword = Parser::Word (input, ctx, Parser::Word::PrecedingLinebreakRequired);
    
    if (keyword.value() == "ELSE") {
      try {
        lookahead = Parameter::Word::peek (input, ctx);
        subcondition = (lookahead.value() == "IF");
      } catch (const Parser::SyntaxError &) {
        subcondition = false;
      }
      
      if (subcondition) {
        lookahead = Parameter::Word::read (input, ctx);
        keyword.append (lookahead);
      }
      m_keywords.append (keyword);
      m_branches.append (
        ConditionalBranchPointer (
          new ConditionalBranch (input, ctx, subcondition)
        )
      );
    } else if (keyword.value() == "ENDIF") {
      m_keywords.append (keyword);
      break;
    } else {
      throw (Parser::SyntaxError (ctx));
    }
  }
}



Obj8::Conditional::~Conditional ()
{
}



Obj8::RecordPointer
Obj8::Conditional::instantiate (StringRef string, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Conditional (string, ctx)));
}



Obj8::String
Obj8::Conditional::name () const
{
  return ("IF");
}



Obj8::String
Obj8::Conditional::toString () const
{
  String retval;
  int i;
  
  for (i=0; i!=m_branches.size(); ++i) {
    retval += m_keywords[i].toString ();
    retval += m_branches[i]->toString ();
  }
  retval += m_keywords[i].toString();
  
  return (retval);
}



void
Obj8::Conditional::accept (AbstractVisitor *visitor, bool visit_all_branches)
{
  bool satisfied;
  
  Q_FOREACH (ConditionalBranchPointer branch, m_branches) {
    satisfied = branch->accept (visitor, visit_all_branches);
    if (satisfied and not visit_all_branches) {
      break;
    }
  }
}
