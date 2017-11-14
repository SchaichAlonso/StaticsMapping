#pragma once

#include <QtGlobal>

#include <Obj8/AbstractBlend.hpp>

namespace Obj8
{
  namespace Global
  {
    struct NoBlend : AbstractBlend
    {
      NoBlend ();
      NoBlend (StringRef, Parser::LexerContext *);
      virtual ~NoBlend ();
      
      virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
      virtual String        name () const Q_DECL_OVERRIDE;
    };
  }
}
 
