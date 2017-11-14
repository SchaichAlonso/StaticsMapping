#include "Ambient.hpp"



Obj8::Command::State::Ambient::Ambient ()
  : AbstractMaterial ()
{
}



Obj8::Command::State::Ambient::Ambient (StringRef str, Parser::LexerContext *ctx)
  : AbstractMaterial (str, ctx)
{
}



Obj8::Command::State::Ambient::~Ambient ()
{
}



Obj8::RecordPointer
Obj8::Command::State::Ambient::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Ambient (str, ctx)));
}



Obj8::String
Obj8::Command::State::Ambient::name () const
{
  return ("ATTR_ambient_rgb");
}
