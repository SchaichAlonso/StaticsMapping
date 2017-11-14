#pragma once

#include <QtGlobal>

#include "Parameter/Number.hpp"

#include "Obj8.hpp"
#include "Record.hpp"

namespace Obj8
{
  struct AbstractSmoke : Record
  {
    AbstractSmoke ();
    AbstractSmoke (StringRef, Parser::LexerContext *);
    virtual ~AbstractSmoke ();
    
    virtual String toString () const Q_DECL_OVERRIDE;
    
  /* protected: */
    Parameter::Number m_x, m_y, m_z, m_s;
  };
}
