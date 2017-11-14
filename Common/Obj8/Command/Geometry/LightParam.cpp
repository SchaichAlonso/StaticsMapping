#include "LightParam.hpp"


Obj8::Command::Geometry::LightParam::LightParam ()
  : Record ()
  , m_name ()
  , m_x (), m_y (), m_z ()
  , m_params ()
{
}



Obj8::Command::Geometry::LightParam::LightParam (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_name (str, ctx)
  , m_x (str, ctx), m_y (str, ctx), m_z (str, ctx)
  , m_params (str, ctx, Parameter::Word::EmptyWordAllowed | Parameter::Word::ConsumeRemainingLine)
{
}



Obj8::Command::Geometry::LightParam::~LightParam ()
{
}



void
Obj8::Command::Geometry::LightParam::accept (AbstractVisitor *visitor, bool)
{
  visitor->visit (this);
}



Obj8::RecordPointer
Obj8::Command::Geometry::LightParam::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new LightParam (str, ctx)));
}



Obj8::String
Obj8::Command::Geometry::LightParam::name () const
{
  return ("LIGHT_PARAM");
}



Obj8::String
Obj8::Command::Geometry::LightParam::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_name.toString ();
  retval += m_x.toString () + m_y.toString () + m_z.toString ();
  retval += m_params.toString ();
  
  return (retval);
}
