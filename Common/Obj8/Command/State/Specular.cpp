#include "Specular.hpp"



Obj8::Command::State::Specular::Specular ()
  : AbstractMaterial ()
{
}



Obj8::Command::State::Specular::Specular (StringRef str, Parser::LexerContext *ctx)
  : AbstractMaterial (str, ctx)
{
}



Obj8::Command::State::Specular::~Specular ()
{
}



Obj8::RecordPointer
Obj8::Command::State::Specular::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Specular (str, ctx)));
}



Obj8::String
Obj8::Command::State::Specular::name () const
{
  return ("ATTR_specular_rgb");
}
