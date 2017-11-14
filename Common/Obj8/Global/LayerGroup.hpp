#pragma once

#include <QtGlobal>

#include <Obj8/Obj8.hpp>
#include <Obj8/AbstractLayerGroup.hpp>

namespace Obj8
{
  namespace Global
  {
    struct LayerGroup : AbstractLayerGroup
    {
      LayerGroup ();
      LayerGroup (StringRef, Parser::LexerContext *);
      virtual ~LayerGroup ();
      
      virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
      virtual String        name () const Q_DECL_OVERRIDE;
    };
  }
}
