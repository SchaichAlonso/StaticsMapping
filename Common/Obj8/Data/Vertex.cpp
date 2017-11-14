#include "Vertex.hpp"



Obj8::Data::Vertex::Vertex ()
  : AbstractVertex ()
  , nx()
  , ny()
  , nz()
  , s()
  , t()
{
}



Obj8::Data::Vertex::Vertex (StringRef str, Parser::LexerContext *ctx)
  : AbstractVertex (str, ctx)
  , nx(str, ctx)
  , ny(str, ctx)
  , nz(str, ctx)
  ,  s(str, ctx)
  ,  t(str, ctx)
{
}



Obj8::Data::Vertex::~Vertex ()
{
}



void
Obj8::Data::Vertex::accept (AbstractVisitor *visitor, bool)
{
  visitor->visit (this);
}



Obj8::RecordPointer
Obj8::Data::Vertex::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Vertex (str, ctx)));
}



Obj8::String
Obj8::Data::Vertex::name () const
{
  return ("VT");
}



Obj8::String
Obj8::Data::Vertex::toString () const
{
  String retval;
  
  retval  = AbstractVertex::toString ();
  retval += nx.toString () + ny.toString () + nz.toString ();
  retval +=  s.toString () +  t.toString ();
  
  return (retval);
}
