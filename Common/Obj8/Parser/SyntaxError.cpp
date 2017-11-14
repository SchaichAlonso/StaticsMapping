#include "SyntaxError.hpp"
#include "Word.hpp"



Obj8::Parser::SyntaxError::SyntaxError (const LexerContext &lc)
  : m_info (lc)
{
}



Obj8::Parser::SyntaxError::SyntaxError (const LexerContext *lc)
  : m_info (lc)
{
}



Obj8::Parser::SyntaxError::SyntaxError (const SyntaxError &other)
  : m_info (other.m_info)
{
}



Obj8::Parser::SyntaxError::SyntaxError (const Word &w)
  : m_info (w.lexerContext ())
{
}



Obj8::Parser::SyntaxError::~SyntaxError ()
{
}



const Obj8::Parser::LexerContext &
Obj8::Parser::SyntaxError::info () const
{
  return (m_info);
}
