#include "AbstractMaterial.hpp"



Obj8::AbstractMaterial::AbstractMaterial ()
  : Record ()
  , m_r ()
  , m_g ()
  , m_b ()
{
}



Obj8::AbstractMaterial::AbstractMaterial (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_r (str, ctx)
  , m_g (str, ctx)
  , m_b (str, ctx)
{
}



Obj8::AbstractMaterial::~AbstractMaterial ()
{
}



void
Obj8::AbstractMaterial::accept (AbstractVisitor *, bool)
{
}



Obj8::String
Obj8::AbstractMaterial::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_r.toString ();
  retval += m_g.toString ();
  retval += m_b.toString ();
  
  return (retval);
}
