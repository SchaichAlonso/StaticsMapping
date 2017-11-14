#include "LightLevel.hpp"



Obj8::Command::State::LightLevel::LightLevel ()
  : Record ()
  , m_v1 ()
  , m_v2 ()
  , m_dataref ()
{
}



Obj8::Command::State::LightLevel::LightLevel (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_v1 (str, ctx)
  , m_v2 (str, ctx)
  , m_dataref (str, ctx)
{
}



Obj8::Command::State::LightLevel::~LightLevel ()
{
}



void
Obj8::Command::State::LightLevel::accept (AbstractVisitor *, bool)
{
}



Obj8::RecordPointer
Obj8::Command::State::LightLevel::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new LightLevel (str, ctx)));
}



Obj8::String
Obj8::Command::State::LightLevel::name () const
{
  return ("ATTR_light_level");
}



Obj8::String
Obj8::Command::State::LightLevel::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_v1.toString ();
  retval += m_v2.toString ();
  retval += m_dataref.toString ();
  
  return (retval);
}
