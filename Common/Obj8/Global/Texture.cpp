#include "Texture.hpp"




Obj8::Global::Texture::Texture ()
  : AbstractTexture ()
{
}



Obj8::Global::Texture::Texture (StringRef str, Parser::LexerContext *ctx)
  : AbstractTexture (str, ctx, Parser::Word::EmptyWordAllowed)
{
}



Obj8::Global::Texture::~Texture ()
{
}



Obj8::RecordPointer
Obj8::Global::Texture::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Texture (str, ctx)));
}



void
Obj8::Global::Texture::accept (AbstractVisitor *visitor, bool)
{
  visitor->visit (this);
}



Obj8::String
Obj8::Global::Texture::name () const
{
  return ("TEXTURE");
}
