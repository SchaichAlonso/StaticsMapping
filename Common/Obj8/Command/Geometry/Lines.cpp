#include "Lines.hpp"



Obj8::Command::Geometry::Lines::Lines ()
  : Record ()
  , m_offset ()
  , m_count ()
{
}



Obj8::Command::Geometry::Lines::Lines (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_offset (str, ctx)
  , m_count (str, ctx)
{
  int count;
  
  count = m_count.toInt ();
  
  if ((count % 2) != 0) {
    throw (Parser::SyntaxError (m_offset.lexerContext()));
  }
}



Obj8::Command::Geometry::Lines::~Lines ()
{
}



int
Obj8::Command::Geometry::Lines::count () const
{
  return (m_count.toInt ());
}



int
Obj8::Command::Geometry::Lines::offset () const
{
  return (m_offset.toInt ());
}



void
Obj8::Command::Geometry::Lines::accept (AbstractVisitor *visitor, bool)
{
  visitor->visit (this);
}



Obj8::RecordPointer
Obj8::Command::Geometry::Lines::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Lines (str, ctx)));
}



Obj8::String
Obj8::Command::Geometry::Lines::name () const
{
  return ("LINES");
}



Obj8::String
Obj8::Command::Geometry::Lines::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_offset.toString ();
  retval += m_count.toString ();
  
  return (retval);
}
