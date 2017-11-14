#include "Emission.hpp"



Obj8::Command::State::Emission::Emission ()
  : AbstractMaterial ()
{
}



Obj8::Command::State::Emission::Emission (StringRef str, Parser::LexerContext *ctx)
  : AbstractMaterial (str, ctx)
{
}



Obj8::Command::State::Emission::~Emission ()
{
}



Obj8::RecordPointer
Obj8::Command::State::Emission::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Emission (str, ctx)));
}



Obj8::String
Obj8::Command::State::Emission::name () const
{
  return ("ATTR_emission_rgb");
}
