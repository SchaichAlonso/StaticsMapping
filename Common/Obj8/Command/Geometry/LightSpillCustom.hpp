#pragma once

#include <QtGlobal>

#include <Obj8/Parameter/Number.hpp>
#include <Obj8/Parameter/Ratio.hpp>
#include <Obj8/Parameter/Word.hpp>
#include <Obj8/Record.hpp>

namespace Obj8
{
  namespace Command
  {
    namespace Geometry
    {
      struct LightSpillCustom : Record
      {
        LightSpillCustom ();
        LightSpillCustom (StringRef, Parser::LexerContext *);
        virtual ~LightSpillCustom ();
        
        virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
        virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
        virtual String        name () const Q_DECL_OVERRIDE;
        virtual String        toString () const Q_DECL_OVERRIDE;
        
        Parameter::Number     m_x, m_y, m_z;
        Parameter::Ratio      m_r, m_g, m_b, m_a;
        Parameter::Number     m_s;
        Parameter::Ratio      m_dx, m_dy, m_dz;
        Parameter::Number     m_semi;
        Parameter::Word       m_dataref;
      };
    }
  }
}
