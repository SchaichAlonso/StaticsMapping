#include "NoShadow.hpp"



Obj8::Global::NoShadow::NoShadow ()
  : Record ()
{
}



Obj8::Global::NoShadow::NoShadow (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
{
}



Obj8::Global::NoShadow::~NoShadow ()
{
}



void
Obj8::Global::NoShadow::accept (AbstractVisitor *, bool)
{
}



Obj8::RecordPointer
Obj8::Global::NoShadow::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new NoShadow (str, ctx)));
}



Obj8::String
Obj8::Global::NoShadow::name () const
{
  return ("GLOBAL_no_shadow");
}
