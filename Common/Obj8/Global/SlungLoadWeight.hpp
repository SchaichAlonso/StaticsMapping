#pragma once

#include <QtGlobal>

#include <Obj8/Parameter/Number.hpp>
#include <Obj8/Record.hpp>

namespace Obj8
{
  namespace Global
  {
    struct SlungLoadWeight : Record
    {
      SlungLoadWeight ();
      SlungLoadWeight (StringRef, Parser::LexerContext *);
      virtual ~SlungLoadWeight ();
      
      virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
      virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
      virtual String        name () const Q_DECL_OVERRIDE;
      virtual String        toString () const Q_DECL_OVERRIDE;
      
    protected:
      Parameter::Number m_weight;
    };
  }
}
