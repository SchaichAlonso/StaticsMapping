#pragma once

#include <QtGlobal>

#include <Obj8/Obj8.hpp>
#include <Obj8/Record.hpp>
#include <Obj8/Parameter/Ratio.hpp>

namespace Obj8
{
  namespace Global
  {
    struct Tint : Record
    {
      Tint ();
      Tint (StringRef, Parser::LexerContext *);
      virtual ~Tint ();
      
      virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
      virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
      virtual String        name () const Q_DECL_OVERRIDE;
      virtual String        toString () const Q_DECL_OVERRIDE;
      
      double albedo () const;
      double emissive () const;
      
    protected:
      Parameter::Ratio m_albedo, m_emissive;
    };
  }
}
