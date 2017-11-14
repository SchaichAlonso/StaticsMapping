#include <Obj8/Parameter/Ratio.hpp>
#include "BumpLevel.hpp"



Obj8::Global::Shader::BumpLevel::BumpLevel ()
  : AbstractShader ()
{
}



Obj8::Global::Shader::BumpLevel::BumpLevel (StringRef str, Parser::LexerContext *ctx)
  : AbstractShader (str, ctx)
{
  bool ok = ((Parameter::Ratio*)&m_ratio)->isValid ();
  
  if (not ok) {
    m_ratio.reject ();
  }
}



Obj8::Global::Shader::BumpLevel::~BumpLevel ()
{
}



Obj8::RecordPointer
Obj8::Global::Shader::BumpLevel::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new BumpLevel (str, ctx)));
}



Obj8::String
Obj8::Global::Shader::BumpLevel::name () const
{
  return ("BUMP_LEVEL");
}
