#include "Parser/SyntaxError.hpp"
#include "Magic.hpp"



Obj8::Magic::Magic ()
  : Word ()
{
}



Obj8::Magic::Magic (StringRef str, Parser::LexerContext *ctx)
  : Word (str, ctx, Parser::Word::PrecedingLinebreakRequired)
{ 
  if (value() != "OBJ") {
    throw (Parser::SyntaxError (ctx));
  }
}



Obj8::Magic::~Magic ()
{
}
