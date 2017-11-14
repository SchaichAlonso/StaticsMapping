#include "AbstractTexture.hpp"



Obj8::AbstractTexture::AbstractTexture ()
  : Record ()
  , m_path ()
{
}



Obj8::AbstractTexture::AbstractTexture (StringRef str, Parser::LexerContext *ctx, int flags)
  : Record (str, ctx)
  , m_path (str, ctx, flags)
{
}



Obj8::AbstractTexture::~AbstractTexture ()
{
}



Obj8::String
Obj8::AbstractTexture::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_path.toString ();
  
  return (retval);
}



void
Obj8::AbstractTexture::setPath (Obj8::StringRef path)
{
  m_path.setValue (path);
}



Obj8::String
Obj8::AbstractTexture::path () const
{
  return (m_path.value ());
}
