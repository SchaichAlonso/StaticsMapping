#include "KeyframeLoop.hpp"



Obj8::Command::Animation::KeyframeLoop::KeyframeLoop ()
  : Record ()
  , m_factor ()
{
}



Obj8::Command::Animation::KeyframeLoop::KeyframeLoop (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_factor (str, ctx)
{
}



Obj8::Command::Animation::KeyframeLoop::~KeyframeLoop ()
{
}



void
Obj8::Command::Animation::KeyframeLoop::accept (AbstractVisitor *, bool)
{
}



Obj8::RecordPointer
Obj8::Command::Animation::KeyframeLoop::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new KeyframeLoop (str, ctx)));
}



Obj8::String
Obj8::Command::Animation::KeyframeLoop::name () const
{
  return ("ANIM_keyframe_loop");
}



Obj8::String
Obj8::Command::Animation::KeyframeLoop::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_factor.toString ();
  
  return (retval);
}
