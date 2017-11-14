#pragma once

#include <QtGlobal>

#include <Obj8/AbstractTexture.hpp>

namespace Obj8
{
  namespace Global
  {
    struct TextureLit : AbstractTexture
    {
      TextureLit ();
      TextureLit (StringRef, Parser::LexerContext *);
      virtual ~TextureLit ();
      
      virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
      virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
      virtual String        name () const Q_DECL_OVERRIDE;
    };
  }
}
