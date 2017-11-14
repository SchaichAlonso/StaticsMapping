#include "TextureDraped.hpp"




Obj8::Global::TextureDraped::TextureDraped ()
  : AbstractTexture ()
{
}



Obj8::Global::TextureDraped::TextureDraped (StringRef str, Parser::LexerContext *ctx)
  : AbstractTexture (str, ctx)
{
}



Obj8::Global::TextureDraped::~TextureDraped ()
{
}



Obj8::RecordPointer
Obj8::Global::TextureDraped::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new TextureDraped (str, ctx)));
}



void
Obj8::Global::TextureDraped::accept (AbstractVisitor *visitor, bool)
{
  visitor->visit (this);
}



Obj8::String
Obj8::Global::TextureDraped::name () const
{
  return ("TEXTURE_DRAPED");
}
