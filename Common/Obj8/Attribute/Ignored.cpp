#include "Ignored.hpp"



Obj8::Attribute::Ignored::Ignored (String name)
  : Record ()
  , m_name (name)
{
}



Obj8::Attribute::Ignored::Ignored (String name, StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_name (name)
{
}



Obj8::Attribute::Ignored::~Ignored ()
{
}



void
Obj8::Attribute::Ignored::accept (AbstractVisitor *, bool)
{
}



Obj8::RecordPointer
Obj8::Attribute::Ignored::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Ignored (m_name, str, ctx)));
}



Obj8::String
Obj8::Attribute::Ignored::name () const
{
  return (m_name);
}
