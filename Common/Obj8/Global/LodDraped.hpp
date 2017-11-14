#pragma once

#include <QtGlobal>

#include <Obj8/Parameter/Integer.hpp>
#include <Obj8/Record.hpp>

namespace Obj8
{
  namespace Global
  {
    struct LodDraped : Record
    {
      LodDraped ();
      LodDraped (StringRef, Parser::LexerContext *);
      virtual ~LodDraped ();
      
      virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
      virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
      virtual String        name () const Q_DECL_OVERRIDE;
      virtual String        toString () const Q_DECL_OVERRIDE;
      
    protected:
      Parameter::Integer m_max;
    };
  }
}
