#include "PointsCounts.hpp"



Obj8::Global::PointsCounts::PointsCounts ()
  : Record ()
  , m_tris ()
  , m_lines ()
  , m_lites ()
  , m_indices ()
{
}



Obj8::Global::PointsCounts::PointsCounts (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_tris (str, ctx)
  , m_lines (str, ctx)
  , m_lites (str, ctx)
  , m_indices (str, ctx)
{
}



Obj8::Global::PointsCounts::~PointsCounts ()
{
}



Obj8::RecordPointer
Obj8::Global::PointsCounts::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new PointsCounts (str, ctx)));
}



Obj8::String
Obj8::Global::PointsCounts::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_tris.toString ();
  retval += m_lines.toString ();
  retval += m_lites.toString ();
  retval += m_indices.toString ();
  
  return (retval);
}



void
Obj8::Global::PointsCounts::accept (AbstractVisitor *visitor, bool)
{
  visitor->visit (this);
}



Obj8::String
Obj8::Global::PointsCounts::name () const
{
  return ("POINT_COUNTS");
}



int
Obj8::Global::PointsCounts::vertices () const
{
  return (m_tris.toInt ());
}



int
Obj8::Global::PointsCounts::lineVertices () const
{
  return (m_lines.toInt ());
}



int
Obj8::Global::PointsCounts::lights () const
{
  return (m_lites.toInt ());
}



int
Obj8::Global::PointsCounts::indices () const
{
  return (m_indices.toInt ());
}
