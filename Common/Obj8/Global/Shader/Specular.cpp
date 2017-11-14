#include <Obj8/Parameter/Ratio.hpp>
#include "Specular.hpp"



Obj8::Global::Shader::Specular::Specular ()
  : AbstractShader ()
{
}



Obj8::Global::Shader::Specular::Specular (StringRef str, Parser::LexerContext *ctx)
  : AbstractShader (str, ctx)
{
  bool ok = ((Parameter::Ratio*)&m_ratio)->isValid ();
  
  if (not ok) {
    m_ratio.reject ();
  }
}



Obj8::Global::Shader::Specular::~Specular ()
{
}



Obj8::RecordPointer
Obj8::Global::Shader::Specular::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Specular (str, ctx)));
}



Obj8::String
Obj8::Global::Shader::Specular::name () const
{
  return ("SPECULAR");
}
 
