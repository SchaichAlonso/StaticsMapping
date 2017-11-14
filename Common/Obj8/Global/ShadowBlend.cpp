#include "ShadowBlend.hpp"



Obj8::Global::ShadowBlend::ShadowBlend ()
  : AbstractBlend ()
{
}



Obj8::Global::ShadowBlend::ShadowBlend (StringRef str, Parser::LexerContext *ctx)
  : AbstractBlend (str, ctx)
{
  if (m_ratio.empty()) {
    m_ratio.reject ();
  }
}



Obj8::Global::ShadowBlend::~ShadowBlend ()
{
}



Obj8::RecordPointer
Obj8::Global::ShadowBlend::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new ShadowBlend (str, ctx)));
}



Obj8::String
Obj8::Global::ShadowBlend::name () const
{
  return ("GLOBAL_shadow_blend");
}
