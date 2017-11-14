#pragma once

#include <QtGlobal>

#include <Obj8/Record.hpp>

namespace Obj8
{
  namespace Global
  {
    namespace Shader
    {
      struct NoShadow : Record
      {
        NoShadow ();
        NoShadow (StringRef, Parser::LexerContext *);
        virtual ~NoShadow ();
        
        virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
        virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
        virtual String        name () const Q_DECL_OVERRIDE;
      };
    }
  }
}
