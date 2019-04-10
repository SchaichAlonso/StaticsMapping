add_library (Obj8 STATIC
    
  Attribute/Ignored.cpp
    
  Command/Action/SmokeBlack.cpp
  Command/Action/SmokeWhite.cpp
  
  Command/Animation/Hide.cpp
  Command/Animation/KeyframeLoop.cpp
  Command/Animation/Rotation.cpp
  Command/Animation/RotationBegin.cpp
  Command/Animation/RotationKey.cpp
  Command/Animation/Show.cpp
  Command/Animation/Translation.cpp
  Command/Animation/TranslationBegin.cpp
  Command/Animation/TranslationKey.cpp
  
  Command/Geometry/LightCustom.cpp
  Command/Geometry/LightNamed.cpp
  Command/Geometry/LightParam.cpp
  Command/Geometry/Lights.cpp
  Command/Geometry/LightSpillCustom.cpp
  Command/Geometry/Lines.cpp
  Command/Geometry/Triangles.cpp
  
  Command/State/Ambient.cpp
  Command/State/CockpitRegion.cpp
  Command/State/Draped.cpp
  Command/State/Emission.cpp
  Command/State/Hard.cpp
  Command/State/HardDeck.cpp
  Command/State/LightLevel.cpp
  Command/State/Lod.cpp
  Command/State/NoBlend.cpp
  Command/State/NoDraped.cpp
  Command/State/NoShadow.cpp
  Command/State/PolygonOffset.cpp
  Command/State/Shadow.cpp
  Command/State/ShadowBlend.cpp
  Command/State/ShinyRat.cpp
  Command/State/Specular.cpp
  
  Data/Index.cpp
  Data/Index10.cpp
  Data/LightVertex.cpp
  Data/LineVertex.cpp
  Data/Vertex.cpp
  
  Global/Shader/BumpLevel.cpp
  Global/Shader/NoBlend.cpp
  Global/Shader/NoShadow.cpp
  Global/Shader/Specular.cpp
  Global/Shader/TwoSided.cpp
  
  Global/CockpitRegion.cpp
  Global/LayerGroup.cpp
  Global/LayerGroupDraped.cpp
  Global/LodDraped.cpp
  Global/NoBlend.cpp
  Global/NoShadow.cpp
  Global/PointsCounts.cpp
  Global/ShadowBlend.cpp
  Global/SlopeLimit.cpp
  Global/SlungLoadWeight.cpp
  Global/Specular.cpp
  Global/Texture.cpp
  Global/TextureDraped.cpp
  Global/TextureLit.cpp
  Global/TextureNormal.cpp
  Global/Tint.cpp
  
  Parameter/Integer.cpp
  Parameter/Number.cpp
  Parameter/Ratio.cpp
  Parameter/Word.cpp
  
  Parser/LexerContext.cpp
  Parser/Number.cpp
  Parser/SyntaxError.cpp
  Parser/Word.cpp
  
  AbstractBlend.cpp
  AbstractHard.cpp
  AbstractIndex.cpp
  AbstractLayerGroup.cpp
  AbstractMaterial.cpp
  AbstractShader.cpp
  AbstractSmoke.cpp
  AbstractSpecular.cpp
  AbstractTexture.cpp
  AbstractToggleVisibility.cpp
  AbstractVertex.cpp
  AbstractVisitor.cpp
  
  Comment.cpp
  Conditional.cpp
  Empty.cpp
  EncodingIndicator.cpp
  File.cpp
  Header.cpp
  Magic.cpp
  Record.cpp
  RecordFactory.cpp
  RecordSequence.cpp
  TextureHashingVisitor.cpp
  Version.cpp
)

target_link_libraries (Obj8
  Common
  
  Qt5::Core
)
