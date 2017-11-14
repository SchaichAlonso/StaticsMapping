#pragma once

#include <QtGlobal>

#include <Obj8/AbstractTexture.hpp>

namespace Obj8
{
  namespace Global
  {
    struct TextureNormal : AbstractTexture
    {
      TextureNormal ();
      TextureNormal (StringRef, Parser::LexerContext *);
      virtual ~TextureNormal ();
      
      virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
      virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
      virtual String        name () const Q_DECL_OVERRIDE;
    };
  }
}
