#include <Obj8/Parser/SyntaxError.hpp>
#include <Obj8/Comment.hpp>

#include "Word.hpp"



Obj8::Parameter::Word::Word ()
  : Parser::Word ()
{
}



Obj8::Parameter::Word::Word (Parser::WordPrivate *dptr)
  : Parser::Word (dptr)
{
}



Obj8::Parameter::Word::Word (const Word &other)
  : Parser::Word (other)
{
}




Obj8::Parameter::Word::Word (StringRef str, Parser::LexerContext *ctx, int flags)
  : Parser::Word (str, ctx, PrecedingLinebreakNotAllowed | flags)
{
  if (Comment::qualified (*this)) {
    throw (Parser::SyntaxError(ctx));
  }
}



Obj8::Parameter::Word::~Word ()
{
}



Obj8::Parameter::Word &
Obj8::Parameter::Word::operator= (const Word& other)
{
  Parser::Word::operator= (other);
  
  return (*this);
}



Obj8::Parameter::Word
Obj8::Parameter::Word::read (StringRef str, Parser::LexerContext *ctx)
{
  return (Word (str, ctx));
}



Obj8::Parameter::Word
Obj8::Parameter::Word::peek (StringRef str, const Parser::LexerContext *ctx)
{
  Parser::LexerContext copy (ctx);
  return (Word (str, &copy));
}
