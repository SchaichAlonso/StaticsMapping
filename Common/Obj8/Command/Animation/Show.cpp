#include "Show.hpp"



Obj8::Command::Animation::Show::Show ()
  : AbstractToggleVisibility ()
{
}



Obj8::Command::Animation::Show::Show (StringRef str, Parser::LexerContext *ctx)
  : AbstractToggleVisibility (str, ctx)
{
}



Obj8::Command::Animation::Show::~Show ()
{
}



Obj8::RecordPointer
Obj8::Command::Animation::Show::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Show (str, ctx)));
}



Obj8::String
Obj8::Command::Animation::Show::name () const
{
  return ("ANIM_show");
}
