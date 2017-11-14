#pragma once

#include <QtGlobal>

#include <Obj8/Obj8.hpp>
#include <Obj8/Record.hpp>
#include <Obj8/Parameter/Ratio.hpp>

namespace Obj8
{
  struct AbstractBlend : Record
  {
    AbstractBlend ();
    AbstractBlend (StringRef, Parser::LexerContext *);
    virtual ~AbstractBlend ();
    
    virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
    virtual String        toString () const Q_DECL_OVERRIDE;
    
    double ratio () const;
    
  protected:
    Parameter::Ratio m_ratio;
  };
}
 
