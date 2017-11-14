#pragma once

#include "Parser/Word.hpp"

#include "Obj8.hpp"

namespace Obj8
{
  struct Magic : Parser::Word
  {
    Magic ();
    Magic (StringRef, Parser::LexerContext *);
    virtual ~Magic ();
  };
}
