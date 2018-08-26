#pragma once

#include <QtGlobal>

#include <Obj8/Parameter/Integer.hpp>
#include <Obj8/Record.hpp>

namespace Obj8
{
  namespace Command
  {
    namespace State
    {
      struct Lod : Record
      {
        Lod ();
        Lod (StringRef, Parser::LexerContext *);
        virtual ~Lod ();
        
        virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
        virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
        virtual String        name () const Q_DECL_OVERRIDE;
        virtual String        toString () const Q_DECL_OVERRIDE;
        
        int nearDistance() const;
        int farDistance() const;
        
      protected:
        Parameter::Integer m_near, m_far;
      };
    }
  }
}
