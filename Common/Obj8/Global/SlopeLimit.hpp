#pragma once

#include <QtGlobal>

#include <Common.hpp>

#include <Obj8/Obj8.hpp>
#include <Obj8/Record.hpp>
#include <Obj8/Parameter/Number.hpp>

namespace Obj8
{
  namespace Global
  {
    struct SlopeLimit : Record
    {
      SlopeLimit ();
      SlopeLimit (StringRef, Parser::LexerContext *);
      virtual ~SlopeLimit ();
      
      virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
      virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
      virtual String        name () const Q_DECL_OVERRIDE;
      virtual String        toString () const Q_DECL_OVERRIDE;
      
    protected:
      Parameter::Number m_pitch_min, m_pitch_max, m_roll_min, m_roll_max;
    };
  }
}
