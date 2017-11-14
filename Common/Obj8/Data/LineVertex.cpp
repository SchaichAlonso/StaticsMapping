#include "LineVertex.hpp"



Obj8::Data::LineVertex::LineVertex ()
  : AbstractVertex ()
  , r(), g(), b()
{
}



Obj8::Data::LineVertex::LineVertex (StringRef str, Parser::LexerContext *ctx)
  : AbstractVertex (str, ctx)
  , r(str, ctx), g(str, ctx), b(str, ctx)
{
}



Obj8::Data::LineVertex::~LineVertex ()
{
}



void
Obj8::Data::LineVertex::accept (AbstractVisitor *visitor, bool)
{
  visitor->visit (this);
}




Obj8::RecordPointer
Obj8::Data::LineVertex::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new LineVertex (str, ctx)));
}



Obj8::String
Obj8::Data::LineVertex::name () const
{
  return ("VLINE");
}



Obj8::String
Obj8::Data::LineVertex::toString () const
{
  String retval;
  
  retval  = AbstractVertex::toString ();
  retval += r.toString () + g.toString () + b.toString ();
  
  return (retval);
}
