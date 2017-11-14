#pragma once

#include "Obj8.hpp"
#include "EncodingIndicator.hpp"
#include "Magic.hpp"
#include "Version.hpp"

namespace Obj8
{
  struct Header
  {
    Header ();
    Header (StringRef, Parser::LexerContext *);
    virtual ~Header ();
    
    String toString () const;
    
  protected:
    EncodingIndicator m_encoding_indicator;
    Version           m_abi_version;
    Magic             m_magic;
  };
}
