#include "AbstractSmoke.hpp"


Obj8::AbstractSmoke::AbstractSmoke ()
  : Record ()
  , m_x ()
  , m_y ()
  , m_z ()
  , m_s ()
{
}



Obj8::AbstractSmoke::AbstractSmoke (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_x (str, ctx)
  , m_y (str, ctx)
  , m_z (str, ctx)
  , m_s (str, ctx)
{
}



Obj8::AbstractSmoke::~AbstractSmoke ()
{
}



Obj8::String
Obj8::AbstractSmoke::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_x.toString ();
  retval += m_y.toString ();
  retval += m_z.toString ();
  retval += m_s.toString ();
  
  return (retval);
}
