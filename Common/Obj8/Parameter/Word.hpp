#pragma once

#include <Obj8/Parser/Word.hpp>

namespace Obj8
{
  namespace Parameter
  {
    struct Word : Parser::Word
    {
      Word ();
      Word (Parser::WordPrivate *);
      Word (const Word &);
      Word (StringRef, Parser::LexerContext * =Q_NULLPTR, int =0);
      virtual ~Word ();
      
      Word &operator= (const Word &other);
      
      static Word read (StringRef, Parser::LexerContext * =Q_NULLPTR);
      static Word peek (StringRef, const Parser::LexerContext * =Q_NULLPTR);
    };
  }
}
