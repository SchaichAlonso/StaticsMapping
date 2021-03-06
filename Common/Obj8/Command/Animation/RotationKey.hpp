#pragma once

#include <QtGlobal>

#include <Obj8/Parameter/Number.hpp>
#include <Obj8/Record.hpp>

namespace Obj8
{
  namespace Command
  {
    namespace Animation
    {
      struct RotationKey : Record
      {
        RotationKey ();
        RotationKey (StringRef, Parser::LexerContext *);
        virtual ~RotationKey ();
        
        virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
        virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
        virtual String        name () const Q_DECL_OVERRIDE;
        virtual String        toString () const Q_DECL_OVERRIDE;
      
      protected:
        Parameter::Number m_value, m_angle;
      };
    }
  }
}
