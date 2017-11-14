#include "SmokeBlack.hpp"


Obj8::Command::Action::SmokeBlack::SmokeBlack ()
  : AbstractSmoke ()
{
}



Obj8::Command::Action::SmokeBlack::SmokeBlack (StringRef str, Parser::LexerContext *ctx)
  : AbstractSmoke (str, ctx)
{
}



Obj8::Command::Action::SmokeBlack::~SmokeBlack ()
{
}



Obj8::RecordPointer
Obj8::Command::Action::SmokeBlack::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new SmokeBlack (str, ctx)));
}


void
Obj8::Command::Action::SmokeBlack::accept (AbstractVisitor *visitor, bool)
{
  visitor->visit (this);
}



Obj8::String
Obj8::Command::Action::SmokeBlack::name () const
{
  return ("smoke_black");
}
