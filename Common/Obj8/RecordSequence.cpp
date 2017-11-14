#include "Parser/SyntaxError.hpp"

#include "Comment.hpp"
#include "Empty.hpp"
#include "RecordFactory.hpp"
#include "RecordSequence.hpp"



Obj8::RecordSequence::RecordSequence ()
  : m_contents ()
{
}



Obj8::RecordSequence::RecordSequence (StringRef str, Parser::LexerContext *ctx, EmptyWordPolicy policy)
  : m_contents ()
{
  const RecordFactory *factory;
  Parser::LexerContext last;
  bool known;
  
  int peek_flags = (policy == AcceptEmpty)? Parser::Word::EmptyWordAllowed : 0;
  
  try {
    
    factory = RecordFactory::instance ();
    
    while (ctx->peek(str) != Parser::LexerContext::EndOfFile) {
      Parser::Word word (Parser::Word::peek (str, ctx, peek_flags));
      StringRef id = word.value ();
    
      known = factory->known (id);
    
      last = *ctx;
      if (known) {
        m_contents.append (factory->instantiate (id, str, ctx));
        continue;
      }
      
      try {
        m_contents.append (RecordPointer (new Comment (str, ctx)));
        continue;
      } catch (const Parser::SyntaxError &) {
        *ctx = last;
      }
      
      if (policy == AcceptEmpty) {
        m_contents.append (RecordPointer(new Empty (str, ctx)));
        continue;
      }
      
      throw (Parser::SyntaxError (ctx));
    }
  } catch (const Parser::SyntaxError &) {
    *ctx = last;
  }
}



Obj8::RecordSequence::~RecordSequence ()
{
  m_contents.clear ();
}



Obj8::String
Obj8::RecordSequence::toString () const
{
  String retval;
  
  Q_FOREACH (RecordPointer r, m_contents) {
    retval += r->toString ();
  }
  
  return (retval);
}



void
Obj8::RecordSequence::accept (AbstractVisitor *visitor, bool all)
{
  Q_FOREACH (RecordPointer record, m_contents) {
    record->accept (visitor, all);
  }
}
