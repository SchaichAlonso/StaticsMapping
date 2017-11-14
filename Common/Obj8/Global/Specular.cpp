#include "Specular.hpp"



Obj8::Global::Specular::Specular ()
  : AbstractSpecular ()
{
}



Obj8::Global::Specular::Specular (StringRef str, Parser::LexerContext *ctx)
  : AbstractSpecular (str, ctx)
{
}



Obj8::Global::Specular::~Specular ()
{
}



Obj8::RecordPointer
Obj8::Global::Specular::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Specular (str, ctx)));
}



Obj8::String
Obj8::Global::Specular::name () const
{
  return ("GLOBAL_specular");
}
