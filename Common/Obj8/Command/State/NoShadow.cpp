#include "NoShadow.hpp"



Obj8::Command::State::NoShadow::NoShadow ()
  : Record ()
{
}



Obj8::Command::State::NoShadow::NoShadow (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
{
}



Obj8::Command::State::NoShadow::~NoShadow ()
{
}



void
Obj8::Command::State::NoShadow::accept (AbstractVisitor *, bool)
{
}



Obj8::RecordPointer
Obj8::Command::State::NoShadow::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new NoShadow (str, ctx)));
}



Obj8::String
Obj8::Command::State::NoShadow::name () const
{
  return ("ATTR_no_shadow");
}
