#include "NoBlend.hpp"



Obj8::Command::State::NoBlend::NoBlend ()
  : AbstractBlend ()
{
}



Obj8::Command::State::NoBlend::NoBlend (StringRef str, Parser::LexerContext *ctx)
  : AbstractBlend (str, ctx)
{
}



Obj8::Command::State::NoBlend::~NoBlend ()
{
}



Obj8::RecordPointer
Obj8::Command::State::NoBlend::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new NoBlend (str, ctx)));
}



Obj8::String
Obj8::Command::State::NoBlend::name () const
{
  return ("ATTR_no_blend");
}
