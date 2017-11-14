#pragma once

#include <QtGlobal>

#include <Obj8/Parameter/Number.hpp>
#include <Obj8/Parameter/Word.hpp>
#include <Obj8/Record.hpp>

namespace Obj8
{
  namespace Command
  {
    namespace Animation
    {
      struct Translation : Record
      {
        Translation ();
        Translation (StringRef, Parser::LexerContext *);
        virtual ~Translation ();
        
        virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
        virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
        virtual String        name () const Q_DECL_OVERRIDE;
        virtual String        toString () const Q_DECL_OVERRIDE;
        
      /* protected: */
        Parameter::Number m_x1, m_y1, m_z1;
        Parameter::Number m_x2, m_y2, m_z2;
        Parameter::Number m_v1, m_v2;
        Parameter::Word   m_dataref;
      };
    }
  }
}
