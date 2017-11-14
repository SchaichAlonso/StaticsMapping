#pragma once

#include <QtGlobal>

#include <Obj8/AbstractToggleVisibility.hpp>

namespace Obj8
{
  namespace Command
  {
    namespace Animation
    {
      struct Show : AbstractToggleVisibility
      {
        Show ();
        Show (StringRef, Parser::LexerContext *);
        virtual ~Show ();
        
        virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
        virtual String        name () const Q_DECL_OVERRIDE;
      };
    }
  }
}
