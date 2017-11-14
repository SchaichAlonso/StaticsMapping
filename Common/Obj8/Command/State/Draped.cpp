#include "Draped.hpp"



Obj8::Command::State::Draped::Draped ()
  : Record ()
{
}



Obj8::Command::State::Draped::Draped (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
{
}



Obj8::Command::State::Draped::~Draped ()
{
}



void
Obj8::Command::State::Draped::accept (AbstractVisitor *visitor, bool)
{
  visitor->visit (this);
}



Obj8::RecordPointer
Obj8::Command::State::Draped::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Draped (str, ctx)));
}



Obj8::String
Obj8::Command::State::Draped::name () const
{
  return ("ATTR_draped");
}
