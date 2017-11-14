#include "TranslationKey.hpp"



Obj8::Command::Animation::TranslationKey::TranslationKey ()
  : Record ()
  , m_value ()
  , m_x (), m_y (), m_z ()
{
}



Obj8::Command::Animation::TranslationKey::TranslationKey (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_value (str, ctx)
  , m_x (str, ctx), m_y (str, ctx), m_z (str, ctx)
{
}



Obj8::Command::Animation::TranslationKey::~TranslationKey ()
{
}



void
Obj8::Command::Animation::TranslationKey::accept (AbstractVisitor *, bool)
{
}



Obj8::RecordPointer
Obj8::Command::Animation::TranslationKey::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new TranslationKey (str, ctx)));
}



Obj8::String
Obj8::Command::Animation::TranslationKey::name () const
{
  return ("ANIM_trans_key");
}



Obj8::String
Obj8::Command::Animation::TranslationKey::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_value.toString ();
  retval += m_x.toString ();
  retval += m_y.toString ();
  retval += m_z.toString ();
  
  return (retval);
}
