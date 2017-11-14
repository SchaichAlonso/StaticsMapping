#include <Obj8/Attribute/Ignored.hpp>

#include <Obj8/Command/Action/SmokeBlack.hpp>
#include <Obj8/Command/Action/SmokeWhite.hpp>

#include <Obj8/Command/Animation/Hide.hpp>
#include <Obj8/Command/Animation/KeyframeLoop.hpp>
#include <Obj8/Command/Animation/Rotation.hpp>
#include <Obj8/Command/Animation/RotationBegin.hpp>
#include <Obj8/Command/Animation/RotationKey.hpp>
#include <Obj8/Command/Animation/Show.hpp>
#include <Obj8/Command/Animation/Translation.hpp>
#include <Obj8/Command/Animation/TranslationBegin.hpp>
#include <Obj8/Command/Animation/TranslationKey.hpp>

#include <Obj8/Command/Geometry/LightCustom.hpp>
#include <Obj8/Command/Geometry/LightNamed.hpp>
#include <Obj8/Command/Geometry/LightParam.hpp>
#include <Obj8/Command/Geometry/LightSpillCustom.hpp>
#include <Obj8/Command/Geometry/Lights.hpp>
#include <Obj8/Command/Geometry/Lines.hpp>
#include <Obj8/Command/Geometry/Triangles.hpp>

#include <Obj8/Command/State/Ambient.hpp>
#include <Obj8/Command/State/CockpitRegion.hpp>
#include <Obj8/Command/State/Draped.hpp>
#include <Obj8/Command/State/Emission.hpp>
#include <Obj8/Command/State/Hard.hpp>
#include <Obj8/Command/State/HardDeck.hpp>
#include <Obj8/Command/State/LightLevel.hpp>
#include <Obj8/Command/State/Lod.hpp>
#include <Obj8/Command/State/NoBlend.hpp>
#include <Obj8/Command/State/NoDraped.hpp>
#include <Obj8/Command/State/NoShadow.hpp>
#include <Obj8/Command/State/PolygonOffset.hpp>
#include <Obj8/Command/State/Shadow.hpp>
#include <Obj8/Command/State/ShadowBlend.hpp>
#include <Obj8/Command/State/ShinyRat.hpp>
#include <Obj8/Command/State/Specular.hpp>

#include <Obj8/Data/Index.hpp>
#include <Obj8/Data/Index10.hpp>
#include <Obj8/Data/LightVertex.hpp>
#include <Obj8/Data/LineVertex.hpp>
#include <Obj8/Data/Vertex.hpp>

#include <Obj8/Global/Shader/BumpLevel.hpp>
#include <Obj8/Global/Shader/NoBlend.hpp>
#include <Obj8/Global/Shader/NoShadow.hpp>
#include <Obj8/Global/Shader/Specular.hpp>
#include <Obj8/Global/Shader/TwoSided.hpp>

#include <Obj8/Global/CockpitRegion.hpp>
#include <Obj8/Global/LayerGroup.hpp>
#include <Obj8/Global/LayerGroupDraped.hpp>
#include <Obj8/Global/LodDraped.hpp>
#include <Obj8/Global/NoBlend.hpp>
#include <Obj8/Global/NoShadow.hpp>
#include <Obj8/Global/PointsCounts.hpp>
#include <Obj8/Global/ShadowBlend.hpp>
#include <Obj8/Global/SlopeLimit.hpp>
#include <Obj8/Global/SlungLoadWeight.hpp>
#include <Obj8/Global/Specular.hpp>
#include <Obj8/Global/Texture.hpp>
#include <Obj8/Global/TextureDraped.hpp>
#include <Obj8/Global/TextureLit.hpp>
#include <Obj8/Global/TextureNormal.hpp>
#include <Obj8/Global/Tint.hpp>
#include <Obj8/Parser/SyntaxError.hpp>

#include "Conditional.hpp"
#include "RecordFactory.hpp"

Obj8::RecordFactory::KnownRecords Obj8::RecordFactory::g_known;



Obj8::RecordFactory::RecordFactory ()
{
  if (not g_known.isEmpty()) {
    return;
  }
  
  
  addType (new Command::Action::SmokeBlack);
  addType (new Command::Action::SmokeWhite);
  
  addType (new Command::Animation::Hide);
  addType (new Command::Animation::KeyframeLoop);
  addType (new Command::Animation::Rotation);
  addType (new Command::Animation::RotationBegin);
  addType (new Command::Animation::RotationKey);
  addType (new Command::Animation::Show);
  addType (new Command::Animation::Translation);
  addType (new Command::Animation::TranslationBegin);
  addType (new Command::Animation::TranslationKey);
  
  addType (new Command::Geometry::LightCustom);
  addType (new Command::Geometry::LightNamed);
  addType (new Command::Geometry::LightParam);
  addType (new Command::Geometry::LightSpillCustom);
  addType (new Command::Geometry::Lights);
  addType (new Command::Geometry::Lines);
  addType (new Command::Geometry::Triangles);
  
  addType (new Command::State::Ambient);
  addType (new Command::State::CockpitRegion);
  addType (new Command::State::Draped);
  addType (new Command::State::Emission);
  addType (new Command::State::Hard);
  addType (new Command::State::HardDeck);
  addType (new Command::State::LightLevel);
  addType (new Command::State::Lod);
  addType (new Command::State::NoBlend);
  addType (new Command::State::NoDraped);
  addType (new Command::State::NoShadow);
  addType (new Command::State::PolygonOffset);
  addType (new Command::State::Shadow);
  addType (new Command::State::ShadowBlend);
  addType (new Command::State::ShinyRat);
  addType (new Command::State::Specular);
  
  addType (new Data::Index);
  addType (new Data::Index10);
  addType (new Data::LightVertex);
  addType (new Data::LineVertex);
  addType (new Data::Vertex);
  
  addType (new Global::Shader::BumpLevel);
  addType (new Global::Shader::NoBlend);
  addType (new Global::Shader::NoShadow);
  addType (new Global::Shader::Specular);
  addType (new Global::Shader::TwoSided);
  
  addType (new Global::CockpitRegion);
  addType (new Global::LayerGroup);
  addType (new Global::LayerGroupDraped);
  addType (new Global::LodDraped);
  addType (new Global::NoBlend);
  addType (new Global::NoShadow);
  addType (new Global::PointsCounts);
  addType (new Global::ShadowBlend);
  addType (new Global::SlopeLimit);
  addType (new Global::SlungLoadWeight);
  addType (new Global::Specular);
  addType (new Global::Texture);
  addType (new Global::TextureDraped);
  addType (new Global::TextureLit);
  addType (new Global::TextureNormal);
  addType (new Global::Tint);
  
  addType (new Attribute::Ignored("NORMAL_METALNESS"));
  addType (new Attribute::Ignored("TILTED"));
  addType (new Attribute::Ignored("REQUIRE_WET"));
  addType (new Attribute::Ignored("REQUIRE_DRY"));
  addType (new Attribute::Ignored("GLOBAL_cockpit_lit"));
  addType (new Attribute::Ignored("ATTR_reset"));
  addType (new Attribute::Ignored("ATTR_cockpit"));
  addType (new Attribute::Ignored("ATTR_no_cockpit"));
  addType (new Attribute::Ignored("ATTR_light_level_reset"));
  addType (new Attribute::Ignored("ATTR_no_hard"));
  addType (new Attribute::Ignored("ATTR_shade_flat"));
  addType (new Attribute::Ignored("ATTR_shade_smooth"));
  addType (new Attribute::Ignored("ATTR_no_depth"));
  addType (new Attribute::Ignored("ATTR_depth"));
  addType (new Attribute::Ignored("ATTR_no_cull"));
  addType (new Attribute::Ignored("ATTR_cull"));
  addType (new Attribute::Ignored("ATTR_blend"));
  addType (new Attribute::Ignored("ATTR_solid_camera"));
  addType (new Attribute::Ignored("ATTR_no_solid_camera"));
  addType (new Attribute::Ignored("ATTR_draw_enable"));
  addType (new Attribute::Ignored("ATTR_draw_disable"));
  addType (new Attribute::Ignored("ANIM_begin"));
  addType (new Attribute::Ignored("ANIM_end"));
  addType (new Attribute::Ignored("ANIM_rotate_end"));
  addType (new Attribute::Ignored("ANIM_trans_end"));
  
  addType (new Conditional);
}



Obj8::RecordFactory::~RecordFactory ()
{
}



bool
Obj8::RecordFactory::known (String key) const
{
  return (g_known.contains (key));
}



Obj8::RecordPointer
Obj8::RecordFactory::instantiate (StringRef data, Parser::LexerContext *ctx) const
{
  Parser::Word word (Parser::Word::peek(data, ctx));
  String id = word.value ();
  
  return (instantiate (id, data, ctx));
}



Obj8::RecordPointer
Obj8::RecordFactory::instantiate (String id, StringRef data, Parser::LexerContext *ctx) const
{
  KnownRecords::ConstIterator i, e;
  
  i = g_known.constFind (id);
  e = g_known.constEnd ();
  
  if (i == e) {
    throw (Parser::SyntaxError (ctx));
  }
  
  return (i.value()->instantiate (data, ctx));
}



void
Obj8::RecordFactory::addType (Record *ptr)
{
  String name = ptr->name ();
  
  Q_ASSERT (not g_known.contains (name));
  
  g_known.insert (name, RecordPointer(ptr));
}



Obj8::RecordFactory *
Obj8::RecordFactory::instance ()
{
  static RecordFactory rf;
  return (&rf);
}
