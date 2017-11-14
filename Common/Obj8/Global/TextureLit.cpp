#include "TextureLit.hpp"




Obj8::Global::TextureLit::TextureLit ()
  : AbstractTexture ()
{
}



Obj8::Global::TextureLit::TextureLit (StringRef str, Parser::LexerContext *ctx)
  : AbstractTexture (str, ctx)
{
}



Obj8::Global::TextureLit::~TextureLit ()
{
}



Obj8::RecordPointer
Obj8::Global::TextureLit::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new TextureLit (str, ctx)));
}



void
Obj8::Global::TextureLit::accept (AbstractVisitor *visitor, bool)
{
  visitor->visit (this);
}



Obj8::String
Obj8::Global::TextureLit::name () const
{
  return ("TEXTURE_LIT");
}
