#include "AbstractVertex.hpp"



Obj8::AbstractVertex::AbstractVertex ()
  : Record ()
  , x ()
  , y ()
  , z ()
{
}



Obj8::AbstractVertex::AbstractVertex (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , x (str, ctx)
  , y (str, ctx)
  , z (str, ctx)
{
}



Obj8::AbstractVertex::~AbstractVertex ()
{
}



Obj8::String
Obj8::AbstractVertex::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += x.toString ();
  retval += y.toString ();
  retval += z.toString ();
  
  return (retval);
}
