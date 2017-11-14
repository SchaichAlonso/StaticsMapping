#include "ShinyRat.hpp"



Obj8::Command::State::ShinyRat::ShinyRat ()
  : AbstractSpecular ()
{
}



Obj8::Command::State::ShinyRat::ShinyRat (StringRef str, Parser::LexerContext *ctx)
  : AbstractSpecular (str, ctx)
{
}



Obj8::Command::State::ShinyRat::~ShinyRat ()
{
}



Obj8::RecordPointer
Obj8::Command::State::ShinyRat::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new ShinyRat (str, ctx)));
}



Obj8::String
Obj8::Command::State::ShinyRat::name () const
{
  return ("ATTR_shiny_rat");
}
