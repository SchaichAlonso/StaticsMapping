#include "AbstractToggleVisibility.hpp"



Obj8::AbstractToggleVisibility::AbstractToggleVisibility ()
  : Record ()
  , m_v1 (), m_v2 ()
  , m_dataref ()
{
}



Obj8::AbstractToggleVisibility::AbstractToggleVisibility (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_v1 (str, ctx), m_v2 (str, ctx)
  , m_dataref (str, ctx)
{
}



Obj8::AbstractToggleVisibility::~AbstractToggleVisibility ()
{
}



void
Obj8::AbstractToggleVisibility::accept (AbstractVisitor *, bool)
{
}



Obj8::String
Obj8::AbstractToggleVisibility::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_v1.toString ();
  retval += m_v2.toString ();
  retval += m_dataref.toString ();
  
  return (retval);
}
