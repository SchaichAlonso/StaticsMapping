#pragma once

#include <QtGlobal>

#include <Obj8/Obj8.hpp>
#include <Obj8/Record.hpp>

namespace Obj8
{
  namespace Attribute
  {
    struct Ignored : Record
    {
      Ignored (String);
      Ignored (String, StringRef, Parser::LexerContext *);
      virtual ~Ignored ();
      
      virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
      virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
      virtual String        name () const Q_DECL_OVERRIDE;
    
    protected:
      String m_name;
    };
  }
}
