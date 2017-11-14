#pragma once

#include <QtGlobal>

#include <Obj8/AbstractToggleVisibility.hpp>

namespace Obj8
{
  namespace Command
  {
    namespace Animation
    {
      struct Hide : AbstractToggleVisibility
      {
        Hide ();
        Hide (StringRef, Parser::LexerContext *);
        virtual ~Hide ();
        
        virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
        virtual String        name () const Q_DECL_OVERRIDE;
      };
    }
  }
}
