#include "RotationKey.hpp"



Obj8::Command::Animation::RotationKey::RotationKey ()
  : Record ()
  , m_value ()
  , m_angle ()
{
}



Obj8::Command::Animation::RotationKey::RotationKey (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_value (str, ctx)
  , m_angle (str, ctx)
{
}



Obj8::Command::Animation::RotationKey::~RotationKey ()
{
}



void
Obj8::Command::Animation::RotationKey::accept (AbstractVisitor *, bool)
{
}



Obj8::RecordPointer
Obj8::Command::Animation::RotationKey::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new RotationKey (str, ctx)));
}



Obj8::String
Obj8::Command::Animation::RotationKey::name () const
{
  return ("ANIM_rotate_key");
}



Obj8::String
Obj8::Command::Animation::RotationKey::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_value.toString ();
  retval += m_angle.toString ();
  
  return (retval);
}
