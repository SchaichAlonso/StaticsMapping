#pragma once

#include <QtGlobal>
#include <QLinkedList>

#include "Parameter/Integer.hpp"
#include "Obj8.hpp"
#include "Record.hpp"

namespace Obj8
{
  struct AbstractIndex : Record
  {
    typedef QLinkedList<Parameter::Integer> Indices;
    
    AbstractIndex ();
    AbstractIndex (StringRef, Parser::LexerContext *);
    virtual ~AbstractIndex ();
    
    int index(int) const;
    Indices indices () const;
    
    virtual String toString () const Q_DECL_OVERRIDE;
    
    Indices m_values;
  protected:
    void readIndex (StringRef, Parser::LexerContext *);
  };
}
