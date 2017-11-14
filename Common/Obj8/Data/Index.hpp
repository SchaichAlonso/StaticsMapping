#pragma once

#include <QtGlobal>

#include <Obj8/AbstractIndex.hpp>

namespace Obj8
{
  namespace Data
  {
    struct Index : AbstractIndex
    {
      Index ();
      Index (StringRef, Parser::LexerContext *);
      virtual ~Index ();
    
      virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
      virtual String        name () const Q_DECL_OVERRIDE;
      virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
    };
  }
}
