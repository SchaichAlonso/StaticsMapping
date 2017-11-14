#pragma once

#include <QSharedPointer>
#include <QString>

#include <Common.hpp>

namespace Obj8
{
#define DECLARE_STRUCT(x) \
  struct x; \
  typedef QSharedPointer<x> x##Pointer
  
  namespace Command
  {
    namespace Action
    {
      DECLARE_STRUCT(SmokeBlack);
      DECLARE_STRUCT(SmokeWhite);
    }
    
    namespace Animation
    {
      DECLARE_STRUCT(Hide);
      DECLARE_STRUCT(KeyframeLoop);
      DECLARE_STRUCT(Rotation);
      DECLARE_STRUCT(RotationBegin);
      DECLARE_STRUCT(RotationKey);
      DECLARE_STRUCT(Show);
      DECLARE_STRUCT(Translation);
      DECLARE_STRUCT(TranslationBegin);
      DECLARE_STRUCT(TranslationKey);
    }
    
    namespace Geometry
    {
      DECLARE_STRUCT(LightCustom);
      DECLARE_STRUCT(LightNamed);
      DECLARE_STRUCT(LightParam);
      DECLARE_STRUCT(LightSpillCustom);
      DECLARE_STRUCT(Lines);
      DECLARE_STRUCT(Lights);
      DECLARE_STRUCT(Triangles);
    }
    
    namespace State
    {
      DECLARE_STRUCT(Ambient);
      DECLARE_STRUCT(CockpitRegion);
      DECLARE_STRUCT(Draped);
      DECLARE_STRUCT(Emission);
      DECLARE_STRUCT(Hard);
      DECLARE_STRUCT(HardDeck);
      DECLARE_STRUCT(LightLevel);
      DECLARE_STRUCT(Lod);
      DECLARE_STRUCT(NoBlend);
      DECLARE_STRUCT(NoDraped);
      DECLARE_STRUCT(NoShadow);
      DECLARE_STRUCT(PolygonOffset);
      DECLARE_STRUCT(Shadow);
      DECLARE_STRUCT(ShadowBlend);
      DECLARE_STRUCT(ShinyRat);
      DECLARE_STRUCT(Specular);
    }
  }
  
  namespace Data
  {
    DECLARE_STRUCT(Index);
    DECLARE_STRUCT(Index10);
    DECLARE_STRUCT(LightVertex);
    DECLARE_STRUCT(LineVertex);
    DECLARE_STRUCT(Vertex);
  }
  
  namespace Global
  {
    namespace Shader
    {
      DECLARE_STRUCT(BumpLevel);
      DECLARE_STRUCT(NoBlend);
      DECLARE_STRUCT(NoShadow);
      DECLARE_STRUCT(Specular);
      DECLARE_STRUCT(TwoSided);
    }
    
    DECLARE_STRUCT(CockpitRegion);
    DECLARE_STRUCT(LayerGroup);
    DECLARE_STRUCT(LayerGroupDraped);
    DECLARE_STRUCT(NoBlend);
    DECLARE_STRUCT(NoShadow);
    DECLARE_STRUCT(PointsCounts);
    DECLARE_STRUCT(ShadowBlend);
    DECLARE_STRUCT(SlopeLimit);
    DECLARE_STRUCT(SlungLoadWeight);
    DECLARE_STRUCT(Specular);
    DECLARE_STRUCT(Texture);
    DECLARE_STRUCT(TextureDraped);
    DECLARE_STRUCT(TextureLit);
    DECLARE_STRUCT(TextureNormal);
    DECLARE_STRUCT(Tint);
  }
  
  DECLARE_STRUCT(AbstractIndex);
  DECLARE_STRUCT(AbstractLayerGroup);
  DECLARE_STRUCT(AbstractSmoke);
  DECLARE_STRUCT(AbstractTexture);
  DECLARE_STRUCT(AbstractVisitor);
  DECLARE_STRUCT(Comment);
  DECLARE_STRUCT(Conditional);
  DECLARE_STRUCT(ConditionalBranch);
  DECLARE_STRUCT(EncodingIndicator);
  DECLARE_STRUCT(Empty);
  DECLARE_STRUCT(Header);
  DECLARE_STRUCT(Ignored);
  DECLARE_STRUCT(File);
  DECLARE_STRUCT(Magic);
  DECLARE_STRUCT(Record);
  DECLARE_STRUCT(Version);
#undef DECLARE_STRUCT
  
  typedef QString        String;
  typedef QString const& StringRef;
}
