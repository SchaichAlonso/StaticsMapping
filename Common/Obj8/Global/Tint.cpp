#include "Tint.hpp"



Obj8::Global::Tint::Tint ()
  : Record ()
  , m_albedo ()
  , m_emissive ()
{
}



Obj8::Global::Tint::Tint (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_albedo (str, ctx)
  , m_emissive (str, ctx)
{
}



Obj8::Global::Tint::~Tint ()
{
}



void
Obj8::Global::Tint::accept (AbstractVisitor *, bool)
{
}



Obj8::RecordPointer
Obj8::Global::Tint::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Tint (str, ctx)));
}



Obj8::String
Obj8::Global::Tint::name () const
{
  return ("GLOBAL_tint");
}



Obj8::String
Obj8::Global::Tint::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_albedo.toString ();
  retval += m_emissive.toString ();
  
  return (retval);
}



double
Obj8::Global::Tint::albedo () const
{
  return (m_albedo.toDouble ());
}



double
Obj8::Global::Tint::emissive () const
{
  return (m_emissive.toDouble ());
}
