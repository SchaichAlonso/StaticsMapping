#pragma once

#include <QtGlobal>

#include <Obj8/Parameter/Ratio.hpp>
#include <Obj8/AbstractVertex.hpp>

namespace Obj8
{
  namespace Data
  {
    struct LineVertex : AbstractVertex
    {
      LineVertex ();
      LineVertex (StringRef, Parser::LexerContext *);
      virtual ~LineVertex ();
    
      virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
      virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
      virtual String        name () const Q_DECL_OVERRIDE;
      virtual String        toString () const Q_DECL_OVERRIDE;
    
      Parameter::Ratio  r, g, b;
    };
  }
}
