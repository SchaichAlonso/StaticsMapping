#include "LightCustom.hpp"


Obj8::Command::Geometry::LightCustom::LightCustom ()
  : Record ()
  , m_x (), m_y (), m_z ()
  , m_r (), m_g (), m_b (), m_a ()
  , m_s ()
  , m_s1 (), m_t1 ()
  , m_s2 (), m_t2 ()
  , m_dataref ()
{
}



Obj8::Command::Geometry::LightCustom::LightCustom (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_x (str, ctx), m_y (str, ctx), m_z (str, ctx)
  , m_r (str, ctx), m_g (str, ctx), m_b (str, ctx), m_a (str, ctx)
  , m_s (str, ctx)
  , m_s1 (str, ctx), m_t1 (str, ctx)
  , m_s2 (str, ctx), m_t2 (str, ctx)
  , m_dataref (str, ctx)
{
}



Obj8::Command::Geometry::LightCustom::~LightCustom ()
{
}



void
Obj8::Command::Geometry::LightCustom::accept (AbstractVisitor *visitor, bool)
{
  visitor->visit (this);
}



Obj8::RecordPointer
Obj8::Command::Geometry::LightCustom::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new LightCustom (str, ctx)));
}



Obj8::String
Obj8::Command::Geometry::LightCustom::name () const
{
  return ("LIGHT_CUSTOM");
}



Obj8::String
Obj8::Command::Geometry::LightCustom::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_x.toString () + m_y.toString () + m_z.toString ();
  retval += m_r.toString () + m_g.toString () + m_b.toString () + m_a.toString ();
  retval += m_s.toString ();
  retval += m_s1.toString () + m_t1.toString ();
  retval += m_s2.toString () + m_t2.toString ();
  retval += m_dataref.toString ();
  
  return (retval);
}
