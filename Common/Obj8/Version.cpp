#include "Version.hpp"



Obj8::Version::Version ()
  : Number ()
  , m_optional_comment ()
{
}



Obj8::Version::Version (StringRef str, Parser::LexerContext *ctx)
  : Number (str, ctx, Parser::Word::PrecedingLinebreakRequired)
  , m_optional_comment ()
{
  Parser::LexerContext backup (ctx);
  try {
    m_optional_comment = Comment (str, ctx, Word::PrecedingLinebreakNotAllowed);
  } catch (const Parser::SyntaxError &) {
    *ctx = backup;
  }
}



Obj8::Version::~Version ()
{
}



Obj8::String
Obj8::Version::toString () const
{
  String retval;
  
  retval  = Number::toString ();
  retval += m_optional_comment.toString ();
  
  return (retval);
}
