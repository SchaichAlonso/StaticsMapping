#pragma once

#include <QtGlobal>

#include "Parser.hpp"

namespace Obj8
{
  namespace Parser
  {
    struct LexerContext
    {
      enum LineEnding {
        Apple,
        IBM
      };
      
      enum SpecialCharacter {
        ShortEndOfLine = 0xe001,
        LongEndOfLine  = 0xe002,
        EndOfFile      = 0xe004,
        EndOfLineMask  = ShortEndOfLine | LongEndOfLine,
        EndMask        = ShortEndOfLine | LongEndOfLine | EndOfFile
      };
      
      LexerContext ();
      LexerContext (const LexerContext &);
      LexerContext (const LexerContext *);
      ~LexerContext ();
      
      LexerContext &operator= (const LexerContext &);
      
      bool atEndOf (StringRef) const;
      int  peek (StringRef) const;
      int  read (StringRef);
      
      String     lineEnding (int) const;
      LineEnding lineEnding () const;
      void setLineEnding (LineEnding);
      
      int line () const;
      int column () const;
      
    protected:
      static int read (LexerContext *, StringRef);
      
      int        m_line, m_col, m_index;
      LineEnding m_eol;
    };
  }
}
