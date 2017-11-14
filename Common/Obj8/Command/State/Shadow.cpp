#include "Shadow.hpp"



Obj8::Command::State::Shadow::Shadow ()
  : Record ()
{
}



Obj8::Command::State::Shadow::Shadow (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
{
}



Obj8::Command::State::Shadow::~Shadow ()
{
}



void
Obj8::Command::State::Shadow::accept (AbstractVisitor *, bool)
{
}



Obj8::RecordPointer
Obj8::Command::State::Shadow::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Shadow (str, ctx)));
}



Obj8::String
Obj8::Command::State::Shadow::name () const
{
  return ("ATTR_shadow");
}
