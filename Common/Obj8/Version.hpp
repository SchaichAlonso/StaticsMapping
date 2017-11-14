#pragma once

#include "Parser/Number.hpp"

#include "Comment.hpp"
#include "Obj8.hpp"

namespace Obj8
{
  struct Version : Parser::Number
  {
    Version ();
    Version (StringRef, Parser::LexerContext *);
    virtual ~Version ();
    
    virtual String toString () const Q_DECL_OVERRIDE;
    
  protected:
    Comment m_optional_comment;
  };
}
