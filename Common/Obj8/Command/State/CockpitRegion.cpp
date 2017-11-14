#include "CockpitRegion.hpp"



Obj8::Command::State::CockpitRegion::CockpitRegion ()
  : Record ()
  , m_region ()
{
}



Obj8::Command::State::CockpitRegion::CockpitRegion (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_region (str, ctx)
{
}



Obj8::Command::State::CockpitRegion::~CockpitRegion ()
{
}



void
Obj8::Command::State::CockpitRegion::accept (AbstractVisitor *, bool)
{
}



Obj8::RecordPointer
Obj8::Command::State::CockpitRegion::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new CockpitRegion (str, ctx)));
}



Obj8::String
Obj8::Command::State::CockpitRegion::name () const
{
  return ("ATTR_cockpit_region");
}



Obj8::String
Obj8::Command::State::CockpitRegion::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_region.toString ();
  
  return (retval);
}
