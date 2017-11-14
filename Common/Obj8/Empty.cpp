#include "Parser/SyntaxError.hpp"
#include "Empty.hpp"



Obj8::Empty::Empty ()
  : Record ()
{
}



Obj8::Empty::Empty (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx, Parser::Word::EmptyWordAllowed)
{
  if (not m_identifier.value().isEmpty()) {
    throw (Parser::SyntaxError (ctx));
  }
  
  /*
   * If there is no indentation either, this instance should not have been
   * asked for.
   */
  if (m_identifier.indentation().isEmpty()) {
    throw (Parser::SyntaxError (ctx));
  }
}



Obj8::Empty::~Empty ()
{
}



Obj8::RecordPointer
Obj8::Empty::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Empty (str, ctx)));
}



Obj8::String
Obj8::Empty::name () const
{
  Q_ASSERT (false);
  return (String ());
}



void
Obj8::Empty::accept (AbstractVisitor *, bool)
{
}
