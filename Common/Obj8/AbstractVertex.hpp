#pragma once

#include <QtGlobal>
#include <QLinkedList>

#include "Parameter/Number.hpp"
#include "Obj8.hpp"
#include "Record.hpp"

namespace Obj8
{
  struct AbstractVertex : Record
  {
    AbstractVertex ();
    AbstractVertex (StringRef, Parser::LexerContext *);
    virtual ~AbstractVertex ();
    
    virtual String toString () const Q_DECL_OVERRIDE;
    
    Parameter::Number x, y, z;
  };
}
