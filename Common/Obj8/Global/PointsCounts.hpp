#pragma once

#include <QtGlobal>

#include <Obj8/Parameter/Integer.hpp>
#include <Obj8/Record.hpp>

namespace Obj8
{
  namespace Global
  {
    struct PointsCounts : Record
    {
      PointsCounts ();
      PointsCounts (StringRef, Parser::LexerContext *);
      virtual ~PointsCounts ();
      
      virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
      virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
      virtual String        name () const Q_DECL_OVERRIDE;
      virtual String        toString () const Q_DECL_OVERRIDE;
      
      int vertices () const;
      int lineVertices () const;
      int lights () const;
      int indices () const;
      
    protected:
      Parameter::Integer m_tris, m_lines, m_lites, m_indices;
    };
  }
}
