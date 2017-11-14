#include "AbstractSpecular.hpp"



Obj8::AbstractSpecular::AbstractSpecular ()
  : Record ()
  , m_ratio ()
{
}



Obj8::AbstractSpecular::AbstractSpecular (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_ratio (str, ctx)
{
}



Obj8::AbstractSpecular::~AbstractSpecular ()
{
}



void
Obj8::AbstractSpecular::accept (AbstractVisitor *, bool)
{
}



Obj8::String
Obj8::AbstractSpecular::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_ratio.toString ();
  
  return (retval);
}



double
Obj8::AbstractSpecular::ratio () const
{
  double retval;
  
  retval = m_ratio.toDouble ();
  
  return (retval);
}
