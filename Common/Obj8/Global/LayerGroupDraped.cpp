#include "LayerGroupDraped.hpp"



Obj8::Global::LayerGroupDraped::LayerGroupDraped ()
  : AbstractLayerGroup ()
{
}



Obj8::Global::LayerGroupDraped::LayerGroupDraped (StringRef str, Parser::LexerContext *ctx)
  : AbstractLayerGroup (str, ctx)
{
}



Obj8::Global::LayerGroupDraped::~LayerGroupDraped ()
{
}



Obj8::RecordPointer
Obj8::Global::LayerGroupDraped::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new LayerGroupDraped (str, ctx)));
}



Obj8::String
Obj8::Global::LayerGroupDraped::name () const
{
  return ("ATTR_layer_group_draped");
}

