#include "SlungLoadWeight.hpp"



Obj8::Global::SlungLoadWeight::SlungLoadWeight ()
  : Record ()
  , m_weight ()
{
}



Obj8::Global::SlungLoadWeight::SlungLoadWeight (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_weight (str, ctx)
{
}



Obj8::Global::SlungLoadWeight::~SlungLoadWeight ()
{
}



void
Obj8::Global::SlungLoadWeight::accept (AbstractVisitor *, bool)
{
}



Obj8::RecordPointer
Obj8::Global::SlungLoadWeight::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new SlungLoadWeight (str, ctx)));
}



Obj8::String
Obj8::Global::SlungLoadWeight::name () const
{
  return ("slung_load_weight");
}



Obj8::String
Obj8::Global::SlungLoadWeight::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_weight.toString ();
  
  return (retval);
}
