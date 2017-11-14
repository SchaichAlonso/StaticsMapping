#include "Hide.hpp"



Obj8::Command::Animation::Hide::Hide ()
  : AbstractToggleVisibility ()
{
}



Obj8::Command::Animation::Hide::Hide (StringRef str, Parser::LexerContext *ctx)
  : AbstractToggleVisibility (str, ctx)
{
}



Obj8::Command::Animation::Hide::~Hide ()
{
}



Obj8::RecordPointer
Obj8::Command::Animation::Hide::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Hide (str, ctx)));
}



Obj8::String
Obj8::Command::Animation::Hide::name () const
{
  return ("ANIM_hide");
}
