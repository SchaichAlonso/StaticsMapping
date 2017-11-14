#include "AbstractBlend.hpp"



Obj8::AbstractBlend::AbstractBlend ()
  : Record ()
  , m_ratio ()
{
}



Obj8::AbstractBlend::AbstractBlend (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_ratio ()
{
  /*
   * Ratio is optional in compatibility mode of ATTR_no_blend
   */
  Parser::LexerContext backup (ctx);
  try {
    m_ratio = Parameter::Ratio (str, ctx);
  } catch (const Parser::SyntaxError &) {
    *ctx = backup;
  }
}



Obj8::AbstractBlend::~AbstractBlend ()
{
}



void
Obj8::AbstractBlend::accept (AbstractVisitor *, bool)
{
}



Obj8::String
Obj8::AbstractBlend::toString () const
{
  String retval;
  
  retval = Record::toString ();
  
  if (m_ratio.isValid ()) {
    retval += m_ratio.toString ();
  }
  
  return (retval);
}



double
Obj8::AbstractBlend::ratio () const
{
  double retval;
  
  if (m_ratio.isValid ()) {
    retval = m_ratio.toDouble ();
  } else {
    retval = 0.5;
  }
  
  return (retval);
}
