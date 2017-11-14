#include "Header.hpp"



Obj8::Header::Header ()
  : m_encoding_indicator ()
  , m_abi_version ()
  , m_magic ()
{
}



Obj8::Header::Header (StringRef str, Parser::LexerContext *ctx)
  : m_encoding_indicator (str, ctx)
  , m_abi_version (str, ctx)
  , m_magic (str, ctx)
{
}



Obj8::Header::~Header ()
{
}



Obj8::String
Obj8::Header::toString () const
{
  String retval;
  
  retval  = m_encoding_indicator.toString ();
  retval += m_abi_version.toString ();
  retval += m_magic.toString ();
  
  return (retval);
}
