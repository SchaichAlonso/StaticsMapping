#pragma once

#include <QtGlobal>

#include <Obj8/Parameter/Word.hpp>
#include <Obj8/Record.hpp>

namespace Obj8
{
  namespace Command
  {
    namespace Animation
    {
      struct TranslationBegin : Record
      {
        TranslationBegin ();
        TranslationBegin (StringRef, Parser::LexerContext *);
        virtual ~TranslationBegin ();
        
        virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
        virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
        virtual String        name () const Q_DECL_OVERRIDE;
        virtual String        toString () const Q_DECL_OVERRIDE;
      
      protected:
        Parameter::Word   m_dataref;
      };
    }
  }
}
