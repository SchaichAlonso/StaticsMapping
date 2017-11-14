#include "AbstractShader.hpp"




Obj8::AbstractShader::AbstractShader ()
  : Record ()
  , m_ratio ()
{
}



Obj8::AbstractShader::AbstractShader (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_ratio (str, ctx)
{
}



Obj8::AbstractShader::~AbstractShader ()
{
}



void
Obj8::AbstractShader::accept (AbstractVisitor *, bool)
{
}



Obj8::String
Obj8::AbstractShader::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_ratio.toString ();
  
  return (retval);
}
