#pragma once

#include "LexerContext.hpp"
#include "Parser.hpp"

namespace Obj8
{
  namespace Parser
  {
    struct SyntaxError
    {
      SyntaxError (const LexerContext &);
      SyntaxError (const LexerContext *);
      SyntaxError (const SyntaxError &);
      SyntaxError (const Word &);
      ~SyntaxError ();
      
      const LexerContext &info () const;
      
    protected:
      LexerContext m_info;
    };
  }
}
