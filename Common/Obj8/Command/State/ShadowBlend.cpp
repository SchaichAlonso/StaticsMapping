#include "ShadowBlend.hpp"



Obj8::Command::State::ShadowBlend::ShadowBlend ()
  : AbstractBlend ()
{
}



Obj8::Command::State::ShadowBlend::ShadowBlend (StringRef str, Parser::LexerContext *ctx)
  : AbstractBlend (str, ctx)
{
  if (m_ratio.empty ()) {
    m_ratio.reject ();
  }
}



Obj8::Command::State::ShadowBlend::~ShadowBlend ()
{
}



Obj8::RecordPointer
Obj8::Command::State::ShadowBlend::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new ShadowBlend (str, ctx)));
}



Obj8::String
Obj8::Command::State::ShadowBlend::name () const
{
  return ("ATTR_shadow_blend");
}
