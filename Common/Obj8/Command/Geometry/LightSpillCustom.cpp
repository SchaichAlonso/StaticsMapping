#include "LightSpillCustom.hpp"


Obj8::Command::Geometry::LightSpillCustom::LightSpillCustom ()
  : Record ()
  , m_x (), m_y (), m_z ()
  , m_r (), m_g (), m_b (), m_a ()
  , m_s ()
  , m_dx (), m_dy (), m_dz ()
  , m_semi ()
  , m_dataref ()
{
}



Obj8::Command::Geometry::LightSpillCustom::LightSpillCustom (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_x (str, ctx), m_y (str, ctx), m_z (str, ctx)
  , m_r (str, ctx), m_g (str, ctx), m_b (str, ctx), m_a (str, ctx)
  , m_s (str, ctx)
  , m_dx (str, ctx), m_dy (str, ctx), m_dz (str, ctx)
  , m_semi (str, ctx)
  , m_dataref (str, ctx)
{
  double x, y, z;
  
  x = m_dx.toDouble();
  y = m_dy.toDouble();
  z = m_dz.toDouble();
  
  if (not qFuzzyCompare(x*x + y*y + z*z, 1)) {
    m_dx.reject ();
  }
}



Obj8::Command::Geometry::LightSpillCustom::~LightSpillCustom ()
{
}



void
Obj8::Command::Geometry::LightSpillCustom::accept (AbstractVisitor *visitor, bool)
{
  visitor->visit (this);
}



Obj8::RecordPointer
Obj8::Command::Geometry::LightSpillCustom::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new LightSpillCustom (str, ctx)));
}



Obj8::String
Obj8::Command::Geometry::LightSpillCustom::name () const
{
  return ("LIGHT_SPILL_CUSTOM");
}



Obj8::String
Obj8::Command::Geometry::LightSpillCustom::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_x.toString ();
  retval += m_y.toString ();
  retval += m_z.toString ();
  retval += m_r.toString ();
  retval += m_g.toString ();
  retval += m_b.toString ();
  retval += m_a.toString ();
  retval += m_s.toString ();
  retval += m_dx.toString ();
  retval += m_dy.toString ();
  retval += m_dz.toString ();
  retval += m_semi.toString ();
  retval += m_dataref.toString ();
  
  return (retval);
}
