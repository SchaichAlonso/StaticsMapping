#include "Rotation.hpp"



Obj8::Command::Animation::Rotation::Rotation ()
  : Record ()
  , m_x (), m_y (), m_z ()
  , m_r1 (), m_r2 ()
  , m_v1 (), m_v2 ()
  , m_dataref ()
{
}



Obj8::Command::Animation::Rotation::Rotation (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_x (str, ctx), m_y (str, ctx), m_z (str, ctx)
  , m_r1 (str, ctx), m_r2 (str, ctx)
  , m_v1 (str, ctx), m_v2 (str, ctx)
  , m_dataref (str, ctx)
{
  if (not Parameter::Number::hasUnitLength (m_x, m_y, m_z)) {
    m_x.reject ();
  }
}



Obj8::Command::Animation::Rotation::~Rotation ()
{
}



void
Obj8::Command::Animation::Rotation::accept (AbstractVisitor *, bool)
{
}



Obj8::RecordPointer
Obj8::Command::Animation::Rotation::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Rotation (str, ctx)));
}



Obj8::String
Obj8::Command::Animation::Rotation::name () const
{
  return ("ANIM_rotate");
}



Obj8::String
Obj8::Command::Animation::Rotation::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_x.toString ();
  retval += m_y.toString ();
  retval += m_z.toString ();
  retval += m_r1.toString ();
  retval += m_r2.toString ();
  retval += m_v1.toString ();
  retval += m_v2.toString ();
  retval += m_dataref.toString ();
  
  return (retval);
}
