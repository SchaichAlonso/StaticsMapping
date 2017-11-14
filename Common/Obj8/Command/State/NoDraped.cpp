#include "NoDraped.hpp"



Obj8::Command::State::NoDraped::NoDraped ()
  : Record ()
{
}



Obj8::Command::State::NoDraped::NoDraped (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
{
}



Obj8::Command::State::NoDraped::~NoDraped ()
{
}



void
Obj8::Command::State::NoDraped::accept (AbstractVisitor *visitor, bool)
{
  visitor->visit (this);
}



Obj8::RecordPointer
Obj8::Command::State::NoDraped::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new NoDraped (str, ctx)));
}



Obj8::String
Obj8::Command::State::NoDraped::name () const
{
  return ("ATTR_no_draped");
}
