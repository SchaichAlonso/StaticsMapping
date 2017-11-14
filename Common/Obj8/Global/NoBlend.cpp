#include "NoBlend.hpp"



Obj8::Global::NoBlend::NoBlend ()
  : AbstractBlend ()
{
}



Obj8::Global::NoBlend::NoBlend (StringRef str, Parser::LexerContext *ctx)
  : AbstractBlend (str, ctx)
{
}



Obj8::Global::NoBlend::~NoBlend ()
{
}



Obj8::RecordPointer
Obj8::Global::NoBlend::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new NoBlend (str, ctx)));
}



Obj8::String
Obj8::Global::NoBlend::name () const
{
  return ("GLOBAL_no_blend");
}
