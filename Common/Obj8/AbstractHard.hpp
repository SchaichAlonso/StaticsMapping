#pragma once

#include <QtGlobal>

#include <Common.hpp>

#include <Obj8/Obj8.hpp>
#include <Obj8/Record.hpp>
#include <Obj8/Parameter/Word.hpp>

namespace Obj8
{
  struct AbstractHard : Record
  {
    enum Surface {
      Default,
      
      Water,
      Concrete,
      Asphalt,
      Grass,
      Dirt,
      Gravel,
      Lakebed,
      Snow,
      Shoulder,
      Blastpad
    };
    
    AbstractHard ();
    AbstractHard (StringRef, Parser::LexerContext *);
    virtual ~AbstractHard ();
    
    virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
    virtual String        toString () const Q_DECL_OVERRIDE;
    
    Surface surface () const;
    void    setSurface (Surface);
    
  protected:
    Parameter::Word m_surface;
    
    static const MappingElement<Surface> g_surfaces[];
  };
}
