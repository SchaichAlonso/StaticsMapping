#pragma once

#include <QVarLengthArray>

#include "LexerContext.hpp"
#include "Parser.hpp"
#include "SyntaxError.hpp"

namespace Obj8
{
  namespace Parser
  {
    struct WordPrivate
    {
      WordPrivate ();
      WordPrivate (StringRef, LexerContext *, int);
      WordPrivate (WordPrivate const &);
      virtual ~WordPrivate ();
      
      virtual WordPrivate *clone () const;
      
      virtual String indentation () const;
      virtual void   setIndentation (StringRef);
      virtual String value () const;
      virtual void   setValue (StringRef);
      virtual void   append (const WordPrivate *);
      virtual bool   isEmpty () const;
      
      virtual String toString () const;
      virtual String restoreLineEndings (StringRef) const;
      
      LexerContext   lexerContext () const;
      
      int            m_references;
      
    protected:
      String         m_indentation, m_value;
      LexerContext   m_lexer_context;
    };
    
    struct Word
    {
      enum Flags
      {
        ConsumeRemainingLine         = 1 << 0,
        PrecedingLinebreakRequired   = 1 << 1,
        PrecedingLinebreakNotAllowed = 1 << 2,
        EmptyWordAllowed             = 1 << 3
      };
      
      Word ();
      Word (WordPrivate *);
      Word (const Word &);
      Word (StringRef, LexerContext * =Q_NULLPTR, int =0);
      virtual ~Word ();
      
      Word &operator= (const Word &other);
      
      static Word    read (StringRef, LexerContext * =Q_NULLPTR, int =0);
      static Word    peek (StringRef, const LexerContext * =Q_NULLPTR, int =0);
      virtual String toString () const;
      
      bool           empty () const;
      void           reject () const;
      LexerContext   lexerContext () const;
      
      String         indentation () const;
      void           setIndentation (StringRef);
      virtual String value () const;
      virtual void   setValue (StringRef);
      
      Word      &append (const Word &);
      
    protected:
      String    restoreLineEndings (StringRef) const;
      void      detach ();
      
      WordPrivate *m_dptr;
    };
  }
}
