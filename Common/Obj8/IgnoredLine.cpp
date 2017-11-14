#include "IgnoredLine.hpp"



Obj8::IgnoredLine::IgnoredLine ()
  : Record ()
{
}


Obj8::IgnoredLine::IgnoredLine (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx, Parser::Word::ConsumeRemainingLine)
{
}



Obj8::IgnoredLine::~IgnoredLine ()
{
}



Obj8::String
Obj8::IgnoredLine::name () const
{
  Q_ASSERT (false);
  return (String ());
}



Obj8::RecordPointer
Obj8::IgnoredLine::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new IgnoredLine (str, ctx)));
}



void
Obj8::IgnoredLine::accept (AbstractVisitor *, bool)
{
}
