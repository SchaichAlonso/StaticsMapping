#include <Obj8/Parameter/Ratio.hpp>
#include "NoBlend.hpp"



Obj8::Global::Shader::NoBlend::NoBlend ()
  : AbstractShader ()
{
}



Obj8::Global::Shader::NoBlend::NoBlend (StringRef str, Parser::LexerContext *ctx)
  : AbstractShader (str, ctx)
{
  bool ok = ((Parameter::Ratio*)&m_ratio)->isValid ();
  
  if (not ok) {
    m_ratio.reject ();
  }
}



Obj8::Global::Shader::NoBlend::~NoBlend ()
{
}



Obj8::RecordPointer
Obj8::Global::Shader::NoBlend::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new NoBlend (str, ctx)));
}



Obj8::String
Obj8::Global::Shader::NoBlend::name () const
{
  return ("NO_BLEND");
}
