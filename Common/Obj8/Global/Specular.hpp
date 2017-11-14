#pragma once

#include <QtGlobal>

#include <Obj8/AbstractSpecular.hpp>

namespace Obj8
{
  namespace Global
  {
    struct Specular : AbstractSpecular
    {
      Specular ();
      Specular (StringRef, Parser::LexerContext *);
      virtual ~Specular ();
      
      virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
      virtual String        name () const Q_DECL_OVERRIDE;
    };
  }
}
