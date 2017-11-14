#pragma once

#include <QtGlobal>

#include <Obj8/Parameter/Number.hpp>
#include <Obj8/AbstractVertex.hpp>

namespace Obj8
{
  namespace Data
  {
    struct Vertex : AbstractVertex
    {
      Vertex ();
      Vertex (StringRef, Parser::LexerContext *);
      virtual ~Vertex ();
    
      virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
      virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
      virtual String        name () const Q_DECL_OVERRIDE;
      virtual String        toString () const Q_DECL_OVERRIDE;
    
      Parameter::Number nx, ny, nz;
      Parameter::Number s, t;
    };
  }
}
