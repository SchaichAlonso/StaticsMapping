#pragma once

#include <QtGlobal>

#include <Common.hpp>

#include <Obj8/Obj8.hpp>
#include <Obj8/Record.hpp>
#include <Obj8/Parameter/Integer.hpp>
#include <Obj8/Parameter/Word.hpp>

namespace Obj8
{
  struct AbstractLayerGroup : Record
  {
    enum Layer {
      Airport,
      Beach,
      Car,
      LightObject,
      Marking,
      Object,
      Road,
      Runway,
      RunwayShoulder,
      Taxiway,
      Terrain,
    };
    
    typedef int Offset;
    
    AbstractLayerGroup ();
    AbstractLayerGroup (StringRef, Parser::LexerContext *);
    virtual ~AbstractLayerGroup ();
    
    virtual void          accept (AbstractVisitor *, bool) Q_DECL_OVERRIDE;
    virtual String        toString () const Q_DECL_OVERRIDE;
    
    Layer  layer () const;
    void   setLayer (Layer);
    
    Offset offset () const;
  protected:
    Parameter::Word    m_layer;
    Parameter::Integer m_offset;
     
    static const MappingElement<Layer> g_layers[];
  };
}
