#include "Triangles.hpp"



Obj8::Command::Geometry::Triangles::Triangles ()
  : Record ()
  , m_offset ()
  , m_count ()
{
}



Obj8::Command::Geometry::Triangles::Triangles (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_offset (str, ctx)
  , m_count (str, ctx)
{
  int count;
  
  count = m_count.toInt ();
  
  if ((count % 3) != 0) {
    throw (Parser::SyntaxError (m_offset.lexerContext()));
  }
}



Obj8::Command::Geometry::Triangles::~Triangles ()
{
}



int
Obj8::Command::Geometry::Triangles::count () const
{
  return (m_count.toInt ());
}



int
Obj8::Command::Geometry::Triangles::offset () const
{
  return (m_offset.toInt ());
}



void
Obj8::Command::Geometry::Triangles::accept (AbstractVisitor *visitor, bool)
{
  visitor->visit (this);
}



Obj8::RecordPointer
Obj8::Command::Geometry::Triangles::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Triangles (str, ctx)));
}



Obj8::String
Obj8::Command::Geometry::Triangles::name () const
{
  return ("TRIS");
}



Obj8::String
Obj8::Command::Geometry::Triangles::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_offset.toString ();
  retval += m_count.toString ();
  
  return (retval);
}
