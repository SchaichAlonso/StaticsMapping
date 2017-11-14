#pragma once

#include <QtGlobal>

#include <Obj8/Parameter/Number.hpp>
#include <Obj8/AbstractVertex.hpp>

namespace Obj8
{
  namespace Data
  {
    struct LightVertex : AbstractVertex
    {
      LightVertex ();
      LightVertex (StringRef, Parser::LexerContext *);
      virtual ~LightVertex ();
    
      virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
      virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
      virtual String        name () const Q_DECL_OVERRIDE;
      virtual String        toString () const Q_DECL_OVERRIDE;
    
      Parameter::Number  r, g, b;
    protected:
      static void rangecheck (const Parameter::Number &);
      static bool special (const Parameter::Number &);
      static Parser::Number g_special[];
    };
  }
}
