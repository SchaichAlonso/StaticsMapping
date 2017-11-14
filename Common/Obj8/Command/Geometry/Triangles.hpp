#pragma once

#include <QtGlobal>

#include <Obj8/Parameter/Integer.hpp>
#include <Obj8/Record.hpp>

namespace Obj8
{
  namespace Command
  {
    namespace Geometry
    {
      struct Triangles : Record
      {
        Triangles ();
        Triangles (StringRef, Parser::LexerContext *);
        virtual ~Triangles ();
        
        int count () const;
        int offset () const;
        
        virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
        virtual RecordPointer instantiate (StringRef, Parser::LexerContext *) const Q_DECL_OVERRIDE;
        virtual String        name () const Q_DECL_OVERRIDE;
        virtual String        toString () const Q_DECL_OVERRIDE;
      
      protected:
        Parameter::Integer m_offset, m_count;
      };
    }
  }
}
