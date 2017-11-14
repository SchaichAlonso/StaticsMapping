#pragma once

#include <QtGlobal>

#include <Obj8/Obj8.hpp>
#include <Obj8/Record.hpp>
#include <Obj8/Parameter/Ratio.hpp>

namespace Obj8
{
  struct AbstractMaterial : Record
  {
    AbstractMaterial ();
    AbstractMaterial (StringRef, Parser::LexerContext *);
    virtual ~AbstractMaterial ();
    
    virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
    virtual String        toString () const Q_DECL_OVERRIDE;
    
  protected:
    Parameter::Number m_r, m_g, m_b;
  };
}
 
