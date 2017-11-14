#pragma once

#include <QtGlobal>

#include <Common.hpp>

#include <Obj8/Obj8.hpp>
#include <Obj8/Record.hpp>
#include <Obj8/Parameter/Integer.hpp>

namespace Obj8
{
  namespace Command
  {
    namespace State
    {
      struct CockpitRegion : Record
      {
        CockpitRegion ();
        CockpitRegion (StringRef, Parser::LexerContext *);
        virtual ~CockpitRegion ();
        
        virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
        virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
        virtual String        name () const Q_DECL_OVERRIDE;
        virtual String        toString () const Q_DECL_OVERRIDE;
        
      protected:
        Parameter::Integer m_region;
      };
    }
  }
}
