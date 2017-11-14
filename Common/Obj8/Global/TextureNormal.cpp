#include "TextureNormal.hpp"




Obj8::Global::TextureNormal::TextureNormal ()
  : AbstractTexture ()
{
}



Obj8::Global::TextureNormal::TextureNormal (StringRef str, Parser::LexerContext *ctx)
  : AbstractTexture (str, ctx)
{
}



Obj8::Global::TextureNormal::~TextureNormal ()
{
}



Obj8::RecordPointer
Obj8::Global::TextureNormal::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new TextureNormal (str, ctx)));
}



void
Obj8::Global::TextureNormal::accept (AbstractVisitor *visitor, bool)
{
  visitor->visit (this);
}



Obj8::String
Obj8::Global::TextureNormal::name () const
{
  return ("TEXTURE_NORMAL");
}
