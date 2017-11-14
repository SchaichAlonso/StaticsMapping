#include "TranslationBegin.hpp"



Obj8::Command::Animation::TranslationBegin::TranslationBegin ()
  : Record ()
  , m_dataref ()
{
}



Obj8::Command::Animation::TranslationBegin::TranslationBegin (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_dataref (str, ctx)
{
}



Obj8::Command::Animation::TranslationBegin::~TranslationBegin ()
{
}



void
Obj8::Command::Animation::TranslationBegin::accept (AbstractVisitor *, bool)
{
}



Obj8::RecordPointer
Obj8::Command::Animation::TranslationBegin::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new TranslationBegin (str, ctx)));
}



Obj8::String
Obj8::Command::Animation::TranslationBegin::name () const
{
  return ("ANIM_trans_begin");
}



Obj8::String
Obj8::Command::Animation::TranslationBegin::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_dataref.toString ();
  
  return (retval);
}
