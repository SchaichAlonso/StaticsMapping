#include "PolygonOffset.hpp"



Obj8::Command::State::PolygonOffset::PolygonOffset ()
  : Record ()
  , m_offset ()
{
}



Obj8::Command::State::PolygonOffset::PolygonOffset (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_offset (str, ctx)
{
  if (offset() < 0) {
    m_offset.reject ();
  }
}



Obj8::Command::State::PolygonOffset::~PolygonOffset ()
{
}



int
Obj8::Command::State::PolygonOffset::offset () const
{
  return (m_offset.toInt ());
}



void
Obj8::Command::State::PolygonOffset::accept (AbstractVisitor *, bool)
{
}



Obj8::RecordPointer
Obj8::Command::State::PolygonOffset::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new PolygonOffset (str, ctx)));
}



Obj8::String
Obj8::Command::State::PolygonOffset::name () const
{
  return ("ATTR_poly_os");
}



Obj8::String
Obj8::Command::State::PolygonOffset::toString () const
{
  String retval;
  
  retval  =  Record::toString ();
  retval += m_offset.toString ();
  
  return (retval);
}
