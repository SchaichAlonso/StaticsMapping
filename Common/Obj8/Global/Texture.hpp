#pragma once

#include <QtGlobal>

#include <Obj8/AbstractTexture.hpp>
#include <Obj8/Record.hpp>

namespace Obj8
{
  namespace Global
  {
    struct Texture : AbstractTexture
    {
      Texture ();
      Texture (StringRef, Parser::LexerContext *);
      virtual ~Texture ();
      
      virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
      virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
      virtual String        name () const Q_DECL_OVERRIDE;
    };
  }
}
