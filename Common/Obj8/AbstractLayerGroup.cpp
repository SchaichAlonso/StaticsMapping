#include "Parser/LexerContext.hpp"

#include "AbstractLayerGroup.hpp"



const MappingElement<Obj8::AbstractLayerGroup::Layer>
Obj8::AbstractLayerGroup::g_layers[] = {
  MAPPING_ELEMENT(Obj8::AbstractLayerGroup, Airport, "airports"),
  MAPPING_ELEMENT(Obj8::AbstractLayerGroup, Beach, "beaches"),
  MAPPING_ELEMENT(Obj8::AbstractLayerGroup, Car, "cars"),
  MAPPING_ELEMENT(Obj8::AbstractLayerGroup, LightObject, "light_objects"),
  MAPPING_ELEMENT(Obj8::AbstractLayerGroup, Marking, "markings"),
  MAPPING_ELEMENT(Obj8::AbstractLayerGroup, Object, "objects"),
  MAPPING_ELEMENT(Obj8::AbstractLayerGroup, Road, "roads"),
  MAPPING_ELEMENT(Obj8::AbstractLayerGroup, Runway, "runways"),
  MAPPING_ELEMENT(Obj8::AbstractLayerGroup, RunwayShoulder, "shoulders"),
  MAPPING_ELEMENT(Obj8::AbstractLayerGroup, Taxiway, "taxiways"),
  MAPPING_ELEMENT(Obj8::AbstractLayerGroup, Terrain, "terrain")
};



Obj8::AbstractLayerGroup::AbstractLayerGroup ()
  : Record ()
  , m_layer ()
  , m_offset ()
{
}



Obj8::AbstractLayerGroup::AbstractLayerGroup (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_layer ()
  , m_offset ()
{
  Parser::LexerContext backup (ctx);
  Offset offset;
  
  try {
    m_layer  = Parameter::Word (str, ctx);
    m_offset = Parameter::Integer (str, ctx);
  } catch (const Parser::SyntaxError &) {
    *ctx = backup;
  }
  
  layer ();
  
  offset = m_offset.toInt ();
  
  if ((offset < -5) or (5 < offset)) {
    m_offset.reject();
  }
}



Obj8::AbstractLayerGroup::~AbstractLayerGroup ()
{
}



void
Obj8::AbstractLayerGroup::accept (AbstractVisitor *, bool)
{
}



Obj8::String
Obj8::AbstractLayerGroup::toString () const
{
  String retval;
  
  retval = Record::toString ();
  
  if (not m_layer.empty()) {
    retval += m_layer.toString ();
    retval += m_offset.toString ();
  }
  
  return (retval);
}



Obj8::AbstractLayerGroup::Layer
Obj8::AbstractLayerGroup::layer () const
{
  Layer retval;
  bool ok;
  
  if (m_layer.empty ())
    return (Object);
  
  ok = string_to_name (g_layers, &retval, m_layer.value());
  if (not ok) {
    m_layer.reject ();
  }
  
  return (retval);
}



void
Obj8::AbstractLayerGroup::setLayer (Layer layer)
{
  QString value;
  bool ok;
  
  ok = name_to_string (g_layers, &value, layer);
  Q_ASSERT (ok);
  
  if (m_layer.indentation().isEmpty()) {
    m_layer.setIndentation(" ");
  }
  m_layer.setValue (value);
}



Obj8::AbstractLayerGroup::Offset
Obj8::AbstractLayerGroup::offset () const
{
  Offset retval;
  
  if (m_layer.empty()) {
    retval = -5;
  } else {
    retval = m_offset.toInt ();
  }
  
  return (retval);
}
