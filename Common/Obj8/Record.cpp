#include "Record.hpp"



Obj8::Record::Record ()
  : m_identifier ()
{
}



Obj8::Record::Record (StringRef str, Parser::LexerContext *ctx, int flags)
  : m_identifier (str, ctx, flags)
{
}



Obj8::Record::~Record ()
{
}



Obj8::String
Obj8::Record::toString () const
{
  return (m_identifier.toString ());
}
