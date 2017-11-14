#include "TwoSided.hpp"



Obj8::Global::Shader::TwoSided::TwoSided ()
  : AbstractShader ()
{
}



Obj8::Global::Shader::TwoSided::TwoSided (StringRef str, Parser::LexerContext *ctx)
  : AbstractShader (str, ctx)
{
  bool ok = m_ratio.isInt ();
  int r = m_ratio.toInt ();
  if (ok) {
    ok = (r == 0) or (r == 1);
  }
  
  if (not ok) {
    m_ratio.reject ();
  }
}



Obj8::Global::Shader::TwoSided::~TwoSided ()
{
}



Obj8::RecordPointer
Obj8::Global::Shader::TwoSided::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new TwoSided (str, ctx)));
}



Obj8::String
Obj8::Global::Shader::TwoSided::name () const
{
  return ("TWO_SIDED");
}
