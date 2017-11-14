#include "LayerGroup.hpp"



Obj8::Global::LayerGroup::LayerGroup ()
  : AbstractLayerGroup ()
{
}



Obj8::Global::LayerGroup::LayerGroup (StringRef str, Parser::LexerContext *ctx)
  : AbstractLayerGroup (str, ctx)
{
  /*
   * ATTR_layer_group_draped can omit the parameters, but ATTR_layer_group
   * cannot.
   */
  if (m_layer.empty ()) {
    m_identifier.reject ();
  }
}



Obj8::Global::LayerGroup::~LayerGroup ()
{
}



Obj8::RecordPointer
Obj8::Global::LayerGroup::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new LayerGroup (str, ctx)));
}



Obj8::String
Obj8::Global::LayerGroup::name () const
{
  return ("ATTR_layer_group");
}

