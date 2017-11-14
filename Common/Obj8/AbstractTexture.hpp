#pragma once

#include <QtGlobal>

#include "Parameter/Word.hpp"

#include "Obj8.hpp"
#include "Record.hpp"

namespace Obj8
{
  struct AbstractTexture : Record
  {
    AbstractTexture ();
    AbstractTexture (StringRef, Parser::LexerContext *, int =0);
    virtual ~AbstractTexture ();
    
    virtual String toString () const;
    
    void   setPath (StringRef);
    String path () const;
    
  protected:
    Parameter::Word m_path;
  };
}
