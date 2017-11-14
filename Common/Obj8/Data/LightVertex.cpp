#include "LightVertex.hpp"


Obj8::Parser::Number
Obj8::Data::LightVertex::g_special[] = {
  Obj8::Parser::Number ("9.7"),
  Obj8::Parser::Number ("9.8"),
  Obj8::Parser::Number ("9.9")
};



Obj8::Data::LightVertex::LightVertex ()
  : AbstractVertex ()
  , r(), g(), b()
{
}



Obj8::Data::LightVertex::LightVertex (StringRef str, Parser::LexerContext *ctx)
  : AbstractVertex (str, ctx)
  , r(str, ctx), g(str, ctx), b(str, ctx)
{
  if (special (r)) {
    if (r.value() != g.value())
      g.reject ();
    if (r.value() != b.value())
      b.reject ();
  } else {
    rangecheck (r);
    rangecheck (g);
    rangecheck (b);
  }
}



Obj8::Data::LightVertex::~LightVertex ()
{
}



void
Obj8::Data::LightVertex::accept (AbstractVisitor *visitor, bool)
{
  visitor->visit (this);
}




Obj8::RecordPointer
Obj8::Data::LightVertex::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new LightVertex (str, ctx)));
}



Obj8::String
Obj8::Data::LightVertex::name () const
{
  return ("VLIGHT");
}



Obj8::String
Obj8::Data::LightVertex::toString () const
{
  String retval;
  
  retval  = AbstractVertex::toString ();
  retval += r.toString () + g.toString () + b.toString ();
  
  return (retval);
}



void
Obj8::Data::LightVertex::rangecheck (const Parameter::Number &c) {
  double d = c.toDouble ();
  
  if ((d < 0) or (1 < d)) {
    c.reject ();
  }
}



bool
Obj8::Data::LightVertex::special (const Parameter::Number &c)
{
  for (size_t i=0; i!=nitems(g_special); ++i) {
    if (c == g_special[i]) {
      return (true);
    }
  }
  
  return (false);
}
