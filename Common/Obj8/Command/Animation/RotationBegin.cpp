#include "RotationBegin.hpp"



Obj8::Command::Animation::RotationBegin::RotationBegin ()
  : Record ()
  , m_x (), m_y (), m_z ()
  , m_dataref ()
{
}



Obj8::Command::Animation::RotationBegin::RotationBegin (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_x (str, ctx), m_y (str, ctx), m_z (str, ctx)
  , m_dataref (str, ctx)
{
}



Obj8::Command::Animation::RotationBegin::~RotationBegin ()
{
}



void
Obj8::Command::Animation::RotationBegin::accept (AbstractVisitor *, bool)
{
}



Obj8::RecordPointer
Obj8::Command::Animation::RotationBegin::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new RotationBegin (str, ctx)));
}



Obj8::String
Obj8::Command::Animation::RotationBegin::name () const
{
  return ("ANIM_rotate_begin");
}



Obj8::String
Obj8::Command::Animation::RotationBegin::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_x.toString ();
  retval += m_y.toString ();
  retval += m_z.toString ();
  retval += m_dataref.toString ();
  
  return (retval);
}
