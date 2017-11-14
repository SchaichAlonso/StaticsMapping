#include "SlopeLimit.hpp"



Obj8::Global::SlopeLimit::SlopeLimit ()
  : Record ()
  , m_pitch_min ()
  , m_pitch_max ()
  , m_roll_min ()
  , m_roll_max ()
{
}




Obj8::Global::SlopeLimit::SlopeLimit (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_pitch_min (str, ctx)
  , m_pitch_max (str, ctx)
  , m_roll_min (str, ctx)
  , m_roll_max (str, ctx)
{
  /*
   * min allowed to be larger than max?
   */
}



Obj8::Global::SlopeLimit::~SlopeLimit ()
{
}



void
Obj8::Global::SlopeLimit::accept (AbstractVisitor *, bool)
{
}



Obj8::RecordPointer
Obj8::Global::SlopeLimit::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new SlopeLimit (str, ctx)));
}



Obj8::String
Obj8::Global::SlopeLimit::name () const
{
  return ("SLOPE_LIMIT");
}



Obj8::String
Obj8::Global::SlopeLimit::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_pitch_min.toString ();
  retval += m_pitch_max.toString ();
  retval += m_roll_min.toString ();
  retval += m_roll_max.toString ();
  
  return (retval);
}
