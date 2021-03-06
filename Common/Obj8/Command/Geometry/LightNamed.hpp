#pragma once

#include <QtGlobal>

#include <Obj8/Parameter/Number.hpp>
#include <Obj8/Parameter/Word.hpp>
#include <Obj8/Record.hpp>

namespace Obj8
{
  namespace Command
  {
    namespace Geometry
    {
      struct LightNamed : Record
      {
        LightNamed ();
        LightNamed (StringRef, Parser::LexerContext *);
        virtual ~LightNamed ();
        
        virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
        virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
        virtual String        name () const Q_DECL_OVERRIDE;
        virtual String        toString () const Q_DECL_OVERRIDE;
        
        Parameter::Word   m_name;
        Parameter::Number m_x, m_y, m_z;
      };
    }
  }
}
