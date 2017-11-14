#pragma once

#include "Parser/LexerContext.hpp"

#include "Obj8.hpp"

namespace Obj8
{
  struct EncodingIndicator
  {
    EncodingIndicator ();
    EncodingIndicator (StringRef, Parser::LexerContext *);
    virtual ~EncodingIndicator ();
    
    String toString () const;
  protected:
    Parser::LexerContext::LineEnding m_eol;
  };
}
