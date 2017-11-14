#pragma once

#include <QtGlobal>

#include "Parameter/Number.hpp"

#include "Obj8.hpp"
#include "Record.hpp"

namespace Obj8
{
  struct AbstractShader : Record
  {
    AbstractShader ();
    AbstractShader (StringRef, Parser::LexerContext *);
    virtual ~AbstractShader ();
    
    virtual void   accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
    virtual String toString () const Q_DECL_OVERRIDE;
    
  protected:
    Parameter::Number m_ratio; /* it is a 0-1 value only for some shaders ... */
  };
}
