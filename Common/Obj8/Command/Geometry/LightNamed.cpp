#include "LightNamed.hpp"



Obj8::Command::Geometry::LightNamed::LightNamed ()
  : Record ()
  , m_name ()
  , m_x ()
  , m_y ()
  , m_z ()
{
}



Obj8::Command::Geometry::LightNamed::LightNamed (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_name (str, ctx)
  , m_x (str, ctx)
  , m_y (str, ctx)
  , m_z (str, ctx)
{
}



Obj8::Command::Geometry::LightNamed::~LightNamed ()
{
}



void
Obj8::Command::Geometry::LightNamed::accept (AbstractVisitor *visitor, bool)
{
  visitor->visit (this);
}



Obj8::RecordPointer
Obj8::Command::Geometry::LightNamed::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new LightNamed (str, ctx)));
}



Obj8::String
Obj8::Command::Geometry::LightNamed::name () const
{
  return ("LIGHT_NAMED");
}



Obj8::String
Obj8::Command::Geometry::LightNamed::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_name.toString ();
  retval += m_x.toString ();
  retval += m_y.toString ();
  retval += m_z.toString ();
  
  return (retval);
}
