#include "SmokeWhite.hpp"


Obj8::Command::Action::SmokeWhite::SmokeWhite ()
  : AbstractSmoke ()
{
}



Obj8::Command::Action::SmokeWhite::SmokeWhite (StringRef str, Parser::LexerContext *ctx)
  : AbstractSmoke (str, ctx)
{
}



Obj8::Command::Action::SmokeWhite::~SmokeWhite ()
{
}



Obj8::RecordPointer
Obj8::Command::Action::SmokeWhite::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new SmokeWhite (str, ctx)));
}


void
Obj8::Command::Action::SmokeWhite::accept (AbstractVisitor *visitor, bool)
{
  visitor->visit (this);
}



Obj8::String
Obj8::Command::Action::SmokeWhite::name () const
{
  return ("smoke_white");
}
