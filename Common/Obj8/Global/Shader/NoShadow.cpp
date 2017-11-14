#include "NoShadow.hpp"



Obj8::Global::Shader::NoShadow::NoShadow ()
  : Record ()
{
}



Obj8::Global::Shader::NoShadow::NoShadow (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
{
}



Obj8::Global::Shader::NoShadow::~NoShadow ()
{
}



void
Obj8::Global::Shader::NoShadow::accept (Obj8::AbstractVisitor*, bool)
{
}



Obj8::RecordPointer
Obj8::Global::Shader::NoShadow::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new NoShadow (str, ctx)));
}



Obj8::String
Obj8::Global::Shader::NoShadow::name () const
{
  return ("NO_SHADOW");
}
