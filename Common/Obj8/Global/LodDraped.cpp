#include "LodDraped.hpp"



Obj8::Global::LodDraped::LodDraped ()
  : Record ()
  , m_max ()
{
}



Obj8::Global::LodDraped::LodDraped (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_max ()
{
  Parser::LexerContext backup (ctx);
  try {
    m_max = Parameter::Integer (str, ctx);
  } catch (const Parser::SyntaxError &) {
    *ctx = backup;
  }
}



Obj8::Global::LodDraped::~LodDraped ()
{
}



void
Obj8::Global::LodDraped::accept (AbstractVisitor *, bool)
{
}



Obj8::RecordPointer
Obj8::Global::LodDraped::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new LodDraped (str, ctx)));
}



Obj8::String
Obj8::Global::LodDraped::name () const
{
  return ("ATTR_LOD_draped");
}



Obj8::String
Obj8::Global::LodDraped::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_max.toString ();
  
  return (retval);
}
