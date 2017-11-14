#include "Hard.hpp"



Obj8::Command::State::Hard::Hard ()
  : AbstractHard ()
{
}



Obj8::Command::State::Hard::Hard (StringRef str, Parser::LexerContext *ctx)
  : AbstractHard (str, ctx)
{
}



Obj8::Command::State::Hard::~Hard ()
{
}



Obj8::RecordPointer
Obj8::Command::State::Hard::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Hard (str, ctx)));
}



Obj8::String
Obj8::Command::State::Hard::name () const
{
  return ("ATTR_hard");
}
