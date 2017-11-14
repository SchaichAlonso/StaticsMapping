#include "Lights.hpp"



Obj8::Command::Geometry::Lights::Lights ()
  : Record ()
  , m_offset ()
  , m_count ()
{
}



Obj8::Command::Geometry::Lights::Lights (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_offset (str, ctx)
  , m_count (str, ctx)
{
}



Obj8::Command::Geometry::Lights::~Lights ()
{
}



int
Obj8::Command::Geometry::Lights::count () const
{
  return (m_count.toInt ());
}



int
Obj8::Command::Geometry::Lights::offset () const
{
  return (m_offset.toInt ());
}



void
Obj8::Command::Geometry::Lights::accept (AbstractVisitor *visitor, bool)
{
  visitor->visit (this);
}



Obj8::RecordPointer
Obj8::Command::Geometry::Lights::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Lights (str, ctx)));
}



Obj8::String
Obj8::Command::Geometry::Lights::name () const
{
  return ("LIGHTS");
}



Obj8::String
Obj8::Command::Geometry::Lights::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_offset.toString ();
  retval += m_count.toString ();
  
  return (retval);
}
