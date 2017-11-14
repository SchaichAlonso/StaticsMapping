#pragma once

#include <QtGlobal>

#include <Obj8/Parameter/Number.hpp>
#include <Obj8/Parameter/Word.hpp>
#include <Obj8/Record.hpp>

namespace Obj8
{
  struct AbstractToggleVisibility : Record
  {
    AbstractToggleVisibility ();
    AbstractToggleVisibility (StringRef, Parser::LexerContext *);
    virtual ~AbstractToggleVisibility ();
    
    virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
    virtual String        toString () const Q_DECL_OVERRIDE;
    
  protected:
    Parameter::Number m_v1, m_v2;
    Parameter::Word   m_dataref;
  };
}
