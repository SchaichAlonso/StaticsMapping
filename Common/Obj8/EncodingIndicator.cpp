#include "Parser/SyntaxError.hpp"

#include "EncodingIndicator.hpp"



Obj8::EncodingIndicator::EncodingIndicator ()
{
}



Obj8::EncodingIndicator::EncodingIndicator (StringRef str, Parser::LexerContext *ctx)
{
  int ch;
  
  Q_CHECK_PTR (ctx);
  
  ch = ctx->peek (str);
  
  switch (ch) {
    case 'A':
      m_eol = Parser::LexerContext::Apple;
      break;
    case 'I':
      m_eol = Parser::LexerContext::IBM;
      break;
    default:
      throw (Parser::SyntaxError (ctx));
  }
  
  ctx->setLineEnding (m_eol);
  ctx->read (str);
}



Obj8::EncodingIndicator::~EncodingIndicator ()
{
}



Obj8::String
Obj8::EncodingIndicator::toString () const
{
  switch (m_eol) {
    case Parser::LexerContext::Apple:
      return ("A");
    case Parser::LexerContext::IBM:
      return ("I");
  }
  Q_ASSERT (false);
}
