#include "Translation.hpp"



Obj8::Command::Animation::Translation::Translation ()
  : Record ()
  , m_x1 (), m_y1 (), m_z1 ()
  , m_x2 (), m_y2 (), m_z2 ()
  , m_v1 (), m_v2 ()
  , m_dataref ()
{
}



Obj8::Command::Animation::Translation::Translation (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_x1 (str, ctx), m_y1 (str, ctx), m_z1 (str, ctx)
  , m_x2 (str, ctx), m_y2 (str, ctx), m_z2 (str, ctx)
  , m_v1 (str, ctx), m_v2 (str, ctx)
  , m_dataref (str, ctx)
{
}



Obj8::Command::Animation::Translation::~Translation ()
{
}



void
Obj8::Command::Animation::Translation::accept (AbstractVisitor *, bool)
{
  /*
   * XXX
   *   Find out how this works.
   */
}



Obj8::RecordPointer
Obj8::Command::Animation::Translation::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Translation (str, ctx)));
}



Obj8::String
Obj8::Command::Animation::Translation::name () const
{
  return ("ANIM_trans");
}



Obj8::String
Obj8::Command::Animation::Translation::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_x1.toString ();
  retval += m_y1.toString ();
  retval += m_z1.toString ();
  retval += m_x2.toString ();
  retval += m_y2.toString ();
  retval += m_z2.toString ();
  retval += m_v1.toString ();
  retval += m_v2.toString ();
  retval += m_dataref.toString ();
  
  return (retval);
}
