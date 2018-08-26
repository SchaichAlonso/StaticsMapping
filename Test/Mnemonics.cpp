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
#include <Obj8/RecordFactory.hpp>
#include <Obj8/RecordSequence.hpp>

#include "Mnemonics.hpp"


Obj8::RecordPointer
Obj8::Test::Mnemonics::accept (QString s) const
{
  RecordPointer ptr;
  Parser::LexerContext x;
  QString src, dst;
  
  src = QString("\n%1").arg(s);
  
  x.setLineEnding (Parser::LexerContext::IBM);
  try {
    ptr = RecordFactory::instance()->instantiate (src, &x);
    dst = ptr->toString ();
    if (src != dst) {
      ptr.reset ();
    }
  } catch (const Parser::SyntaxError &) {
  }
  
  return (ptr);
}


void
Obj8::Test::Mnemonics::ignoredNoArguments (QString s) const
{
  QVERIFY (    accept (QString("%1").arg(s)));
  QVERIFY (not accept (QString("%1 0").arg(s)));
}


template <typename T, int good>
void
Obj8::Test::Mnemonics::index (QString prefix) const
{
  for (int i=0; i!=21; ++i) {
    QStringList params;
    for (int j=0; j!=i; ++j) {
      params.append (QString::number(j));
    }
    QString line = QString("%1 %2").arg(prefix).arg(params.join(" "));
    QVERIFY((i==good) == (bool)accept(line));
    
    for (int j=0; (i==good) and (j!=good); ++j) {
      QStringList text, floats;
      for (int k=0; k!=good; ++k) {
        QString num = QString::number(k);
        if (k==j) {
          text.append ("a");
          floats.append (num + ".0");
        } else {
          text.append (num);
        }
      }
      line = QString("%1 %2").arg(prefix).arg(text.join(" "));
      QVERIFY(not accept(line));
      line = QString("%1 %2").arg(prefix).arg(floats.join(" "));
      QVERIFY(not accept(line));
    }
  }
}


template <typename T>
void
Obj8::Test::Mnemonics::hard (QString prefix) const
{
  typedef T Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (    accept (QString("%1").arg(prefix)));
  QVERIFY (not accept (QString("%1 0").arg(prefix)));
  QVERIFY (    accept (QString("%1 water").arg(prefix)));
  QVERIFY (not accept (QString("%1 water 1").arg(prefix)));
  
  struct {
    QString string;
    Command::State::Hard::Surface name;
  } references[] = {
    { "water",    Command::State::Hard::Water },
    { "concrete", Command::State::Hard::Concrete }, 
    { "asphalt",  Command::State::Hard::Asphalt },
    { "grass",    Command::State::Hard::Grass },
    { "dirt",     Command::State::Hard::Dirt },
    { "gravel",   Command::State::Hard::Gravel },
    { "lakebed",  Command::State::Hard::Lakebed },
    { "snow",     Command::State::Hard::Snow },
    { "shoulder", Command::State::Hard::Shoulder },
    { "blastpad", Command::State::Hard::Blastpad }
  };
  
  for (size_t i=0; i!=sizeof(references)/sizeof(*references); ++i) {
    QString input = QString("ATTR_hard %1").arg(references[i].string);
    RecordPointer   sp = accept (input);
    MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
    QVERIFY (mp);
    QVERIFY (mp->surface() == references[i].name);
  }
}



template <typename T>
void
Obj8::Test::Mnemonics::layerGroup (QString prefix, bool allow_noparams) const
{
  typedef T Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (allow_noparams == (bool)accept (prefix));
  QVERIFY (not accept (QString("%1 terrain").arg(prefix)));
  QVERIFY (    accept (QString("%1 terrain 0").arg(prefix)));
  QVERIFY (    accept (QString("%1 terrain -5").arg(prefix)));
  QVERIFY (not accept (QString("%1 terrain -6").arg(prefix)));
  QVERIFY (    accept (QString("%1 terrain +5").arg(prefix)));
  QVERIFY (not accept (QString("%1 terrain +6").arg(prefix)));
  QVERIFY (not accept (QString("%1 TERRAIN +5").arg(prefix)));
  
  struct {
    QString string;
    typename Mnemonic::Layer name;
  } references[] = {
    { "terrain",       Mnemonic::Terrain },
    { "beaches",       Mnemonic::Beach },
    { "shoulders",     Mnemonic::RunwayShoulder },
    { "taxiways",      Mnemonic::Taxiway },
    { "runways",       Mnemonic::Runway },
    { "markings",      Mnemonic::Marking },
    { "airports",      Mnemonic::Airport },
    { "roads",         Mnemonic::Road },
    { "objects",       Mnemonic::Object },
    { "light_objects", Mnemonic::LightObject },
    { "cars",          Mnemonic::Car }
  };
  
  for (int i=0; i!=nitems(references); ++i) {
    QString          s = QString(QString("%1 %2 2")).arg(prefix).arg(references[i].string);
    RecordPointer   sp = accept (s);
    MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  
    QVERIFY (mp);
    QVERIFY (mp->layer() == references[i].name);
    QVERIFY (mp->offset() == 2);
  }
}


template<typename T>
void
Obj8::Test::Mnemonics::material (QString s) const
{
  typedef T Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept (QString("%1").arg(s)));
  QVERIFY (not accept (QString("%1 1").arg(s)));
  QVERIFY (not accept (QString("%1 1 2").arg(s)));
  QVERIFY (    accept (QString("%1 1 2 3").arg(s)));
  QVERIFY (not accept (QString("%1 a 2 3").arg(s)));
  QVERIFY (not accept (QString("%1 1 a 3").arg(s)));
  QVERIFY (not accept (QString("%1 1 2 a").arg(s)));
  QVERIFY (not accept (QString("%1 1 2 3 4").arg(s)));
  
  RecordPointer   sp = accept (QString("%1 1 2 3").arg(s));
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  /*
   * XXX
   *   data interfaces required
   */
}


template<typename T>
void
Obj8::Test::Mnemonics::smoke (QString s) const
{ 
  typedef T Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept (s));
  QVERIFY (not accept (QString("%1 1").arg(s)));
  QVERIFY (not accept (QString("%1 1 2").arg(s)));
  QVERIFY (not accept (QString("%1 1 2 3").arg(s)));
  QVERIFY (    accept (QString("%1 1 2 3 4").arg(s)));
  QVERIFY (not accept (QString("%1 a 2 3 4").arg(s)));
  QVERIFY (not accept (QString("%1 1 a 3 4").arg(s)));
  QVERIFY (not accept (QString("%1 1 2 a 4").arg(s)));
  QVERIFY (not accept (QString("%1 1 2 3 a").arg(s)));
  QVERIFY (not accept (QString("%1 1 2 3 4 5").arg(s)));
  
  RecordPointer   sp = accept (QString("%1 1 2 3 4").arg(s));
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  /*
   * XXX
   *   data interfaces required
   */
}



template <typename T>
void
Obj8::Test::Mnemonics::texture (QString s, bool allow_empty) const
{
  typedef T Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QCOMPARE (bool(accept(s)), allow_empty);
  
  QVERIFY (    accept (QString("%1 1").arg(s)));
  QVERIFY (    accept (QString("%1 a").arg(s)));
  QVERIFY (not accept (QString("%1 1 2").arg(s)));
  
  RecordPointer   sp = accept (QString("%1 abc").arg(s));
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  QVERIFY (mp->path() == "abc");
}



void
Obj8::Test::Mnemonics::sequence ()
{
  QString references[] = {
    "",
    "#",
    "# x y z ",
    "#ATTR_no_shadow",
    "ATTR_shadow #",
    "ATTR_shadow #hello world",
    "#hello world ATTR_no_shadow",
    "ATTR_LOD 0 1\n # ATTR_no_shadow 0 1\n"
    "# ATTR_no_shadow 0 1\nATTR_LOD 0 1"
  };
  for (int i=0; i!=nitems(references); ++i) {
    Parser::LexerContext ctx;
    QString src, dst;
    
    src = QString("\n%1").arg(references[i]);
    RecordSequence seq (src, &ctx);
    dst = seq.toString ();
    QVERIFY (src == dst);
  }
}



void
Obj8::Test::Mnemonics::conditional ()
{
  QVERIFY (not accept ("IF x"));
  QVERIFY (    accept ("IF x\nENDIF"));
  QVERIFY (    accept ("IF x\n#foo\nENDIF"));
  QVERIFY (not accept ("IF x ENDIF"));
  QVERIFY (not accept ("IF\nENDIF"));
  QVERIFY (    accept ("IF x\nELSE\nENDIF"));
  QVERIFY (    accept ("IF x\nELSE IF x\nENDIF"));
  QVERIFY (not accept ("IF x\nELSE IF\nENDIF"));
  QVERIFY (    accept ("IF NOT x\n#foo\nENDIF"));
  QVERIFY (not accept ("IF NOT #x\nENDIF"));
  QVERIFY (not accept ("ELSE\nENDIF"));
  QVERIFY (not accept ("ENDIF"));
}



void
Obj8::Test::Mnemonics::TEXTURE ()
{
  texture<Global::Texture> (__func__, true);
}



void
Obj8::Test::Mnemonics::TEXTURE_LIT ()
{
  texture<Global::TextureLit> (__func__, false);
}



void
Obj8::Test::Mnemonics::TEXTURE_NORMAL ()
{
  texture<Global::TextureNormal> (__func__, false);
}



void
Obj8::Test::Mnemonics::__TEXTURE_DRAPED ()
{
  texture<Global::TextureDraped> ("TEXTURE_DRAPED", false);
}



void
Obj8::Test::Mnemonics::POINT_COUNTS ()
{
  typedef Global::PointsCounts Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("POINT_COUNTS"));
  QVERIFY (not accept ("POINT_COUNTS 0"));
  QVERIFY (not accept ("POINT_COUNTS 0 1"));
  QVERIFY (not accept ("POINT_COUNTS 0 1 2"));
  QVERIFY (    accept ("POINT_COUNTS 0 1 2 3"));
  QVERIFY (    accept ("POINT_COUNTS -0 -1 -2 -3"));
  QVERIFY (not accept ("POINT_COUNTS a 1 2 3"));
  QVERIFY (not accept ("POINT_COUNTS 0 a 2 3"));
  QVERIFY (not accept ("POINT_COUNTS 0 1 a 3"));
  QVERIFY (not accept ("POINT_COUNTS 0 1 2 a"));
  QVERIFY (not accept ("POINT_COUNTS 0.0 1 2 3"));
  QVERIFY (not accept ("POINT_COUNTS 0 1.0 2 3"));
  QVERIFY (not accept ("POINT_COUNTS 0 1 2.0 3"));
  QVERIFY (not accept ("POINT_COUNTS 0 1 2 3.0"));
  QVERIFY (not accept ("POINT_COUNTS 0 1 2 3 4"));
  
  
  RecordPointer   sp = accept ("POINT_COUNTS 1 2 3 4");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  
  QVERIFY (mp);
  QVERIFY (mp->vertices() == 1);
  QVERIFY (mp->lineVertices() == 2);
  QVERIFY (mp->lights() == 3);
  QVERIFY (mp->indices() == 4);
}



void
Obj8::Test::Mnemonics::slung_load_weight ()
{
  typedef Global::SlungLoadWeight Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("slung_load_weight"));
  QVERIFY (    accept ("slung_load_weight 1"));
  QVERIFY (    accept ("slung_load_weight -1"));
  QVERIFY (    accept ("slung_load_weight 1.75"));
  QVERIFY (not accept ("slung_load_weight 1 2"));
  QVERIFY (not accept ("slung_load_weight a"));
  
  RecordPointer   sp = accept ("slung_load_weight 1234");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  
  QVERIFY (mp);
  /*
   * XXX
   *   data interfaces required
   */
}



void
Obj8::Test::Mnemonics::ATTR_layer_group ()
{
  layerGroup<Global::LayerGroup> (__func__, false);
}



void
Obj8::Test::Mnemonics::COCKPIT_REGION ()
{
  typedef Global::CockpitRegion Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("COCKPIT_REGION"));
  QVERIFY (not accept ("COCKPIT_REGION 0"));
  QVERIFY (not accept ("COCKPIT_REGION 0 1"));
  QVERIFY (not accept ("COCKPIT_REGION 0 1 2"));
  QVERIFY (    accept ("COCKPIT_REGION 0 1 2 3"));
  QVERIFY (    accept ("COCKPIT_REGION 4 5 8 9"));
  QVERIFY (not accept ("COCKPIT_REGION 4 5 2 9"));
  QVERIFY (not accept ("COCKPIT_REGION 4 5 8 3"));
  QVERIFY (not accept ("COCKPIT_REGION 0 1 2 4"));
  QVERIFY (not accept ("COCKPIT_REGION 0 1 3 3"));
  QVERIFY (not accept ("COCKPIT_REGION 0 1 2 3 4"));
  
  RecordPointer   sp = accept ("COCKPIT_REGION 0 1 2 3");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  
  QVERIFY (mp);
  /*
   * XXX
   *   data interfaces required
   */
}



void
Obj8::Test::Mnemonics::GLOBAL_no_blend ()
{
  typedef Global::NoBlend Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (    accept ("GLOBAL_no_blend"));
  QVERIFY (not accept ("GLOBAL_no_blend -1"));
  QVERIFY (    accept ("GLOBAL_no_blend  0"));
  QVERIFY (    accept ("GLOBAL_no_blend  0.999999999"));
  QVERIFY (    accept ("GLOBAL_no_blend  1"));
  QVERIFY (not accept ("GLOBAL_no_blend  1.000000001"));
  QVERIFY (not accept ("GLOBAL_no_blend  a"));
  QVERIFY (not accept ("GLOBAL_no_blend  0 1"));
  
  RecordPointer   sp = accept ("GLOBAL_no_blend  0.9");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  QVERIFY (qFuzzyCompare(mp->ratio(), 0.9));
}



void
Obj8::Test::Mnemonics::GLOBAL_shadow_blend ()
{
  typedef Global::ShadowBlend Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("GLOBAL_shadow_blend"));
  QVERIFY (not accept ("GLOBAL_shadow_blend -1"));
  QVERIFY (    accept ("GLOBAL_shadow_blend  0"));
  QVERIFY (    accept ("GLOBAL_shadow_blend  0.9"));
  QVERIFY (    accept ("GLOBAL_shadow_blend  1"));
  QVERIFY (not accept ("GLOBAL_shadow_blend  2"));
  QVERIFY (not accept ("GLOBAL_shadow_blend  a"));
  QVERIFY (not accept ("GLOBAL_shadow_blend  0 1"));
  
  RecordPointer   sp = accept ("GLOBAL_shadow_blend  0.9");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  QVERIFY (qFuzzyCompare(mp->ratio(), 0.9));
}



/*
 * XXX
 *   Merge this with ATTR_shiny_rat
 */
void
Obj8::Test::Mnemonics::GLOBAL_specular ()
{
  typedef Global::Specular Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("GLOBAL_specular"));
  QVERIFY (not accept ("GLOBAL_specular a"));
  QVERIFY (    accept ("GLOBAL_specular 0"));
  QVERIFY (    accept ("GLOBAL_specular 0.1"));
  QVERIFY (    accept ("GLOBAL_specular 0.75"));
  QVERIFY (    accept ("GLOBAL_specular 1.00"));
  QVERIFY (not accept ("GLOBAL_specular 1.01"));
  QVERIFY (not accept ("GLOBAL_specular 1 2"));
  
  RecordPointer   sp = accept ("GLOBAL_specular 0.23");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  
  QVERIFY (qFuzzyCompare(mp->ratio(), 0.23));
}



void
Obj8::Test::Mnemonics::GLOBAL_tint ()
{
  typedef Global::Tint Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("GLOBAL_tint"));
  QVERIFY (not accept ("GLOBAL_tint  0"));
  QVERIFY (    accept ("GLOBAL_tint  0  1"));
  QVERIFY (not accept ("GLOBAL_tint -1  1"));
  QVERIFY (not accept ("GLOBAL_tint  0 -1"));
  QVERIFY (not accept ("GLOBAL_tint  2  1"));
  QVERIFY (not accept ("GLOBAL_tint  0  2"));
  QVERIFY (not accept ("GLOBAL_tint  0  0 0"));
  
  RecordPointer   sp = accept ("GLOBAL_tint 0.234 0.567");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  QVERIFY (qFuzzyCompare(mp->albedo(), 0.234));
  QVERIFY (qFuzzyCompare(mp->emissive(), 0.567));
}



void
Obj8::Test::Mnemonics::GLOBAL_no_shadow ()
{
  typedef Global::NoShadow Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (    accept ("GLOBAL_no_shadow"));
  QVERIFY (not accept ("GLOBAL_no_shadow  0"));
  
  RecordPointer   sp = accept ("GLOBAL_no_shadow");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
}



void
Obj8::Test::Mnemonics::SLOPE_LIMIT ()
{
  typedef Global::SlopeLimit Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("SLOPE_LIMIT"));
  QVERIFY (not accept ("SLOPE_LIMIT 0"));
  QVERIFY (not accept ("SLOPE_LIMIT 0   1"));
  QVERIFY (not accept ("SLOPE_LIMIT 0   1   2"));
  QVERIFY (    accept ("SLOPE_LIMIT 0   1   2   3"));
  QVERIFY (    accept ("SLOPE_LIMIT 0.0 1.0 2.0 3.0"));
  QVERIFY (not accept ("SLOPE_LIMIT a   1   2   3"));
  QVERIFY (not accept ("SLOPE_LIMIT 0   a   2   3"));
  QVERIFY (not accept ("SLOPE_LIMIT 0   1   a   3"));
  QVERIFY (not accept ("SLOPE_LIMIT 0   1   2   a"));
  QVERIFY (not accept ("SLOPE_LIMIT 0   1   2   3   4"));
  
  RecordPointer   sp = accept ("SLOPE_LIMIT 0   1   2   3");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  /*
   * XXX
   *   data interfaces required
   */
}



void
Obj8::Test::Mnemonics::TILTED ()
{
  ignoredNoArguments (__func__);
}



void
Obj8::Test::Mnemonics::REQUIRE_WET ()
{
  ignoredNoArguments (__func__);
}



void
Obj8::Test::Mnemonics::REQUIRE_DRY ()
{
  ignoredNoArguments (__func__);
}



void
Obj8::Test::Mnemonics::ATTR_layer_group_draped ()
{
  typedef Global::LayerGroupDraped Mnemonic;
  
  layerGroup<Mnemonic> (__func__, true);
  
  QSharedPointer<Mnemonic> mp = qSharedPointerDynamicCast<Mnemonic> (accept(__func__));
  QVERIFY (mp);
  QVERIFY (mp->layer() == Mnemonic::Object);
  QVERIFY (mp->offset() == -5);
}



void
Obj8::Test::Mnemonics::ATTR_LOD_draped ()
{
  typedef Global::LodDraped Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("ATTR_LOD_draped"));
  QVERIFY (    accept ("ATTR_LOD_draped 0"));
  QVERIFY (not accept ("ATTR_LOD_draped a"));
  QVERIFY (not accept ("ATTR_LOD_draped 0 1"));
  
  RecordPointer   sp = accept ("ATTR_LOD_draped 0");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  /*
   * XXX
   *   data interfaces required
   */
}



void
Obj8::Test::Mnemonics::GLOBAL_cockpit_lit ()
{
  ignoredNoArguments (__func__);
}



void
Obj8::Test::Mnemonics::VT ()
{
  typedef Data::Vertex Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("VT"));
  QVERIFY (not accept ("VT  0"));
  QVERIFY (not accept ("VT  0   1"));
  QVERIFY (not accept ("VT  0   1   2"));
  QVERIFY (not accept ("VT  0   1   2   3"));
  QVERIFY (not accept ("VT  0   1   2   3   4"));
  QVERIFY (not accept ("VT  0   1   2   3   4   5"));
  QVERIFY (not accept ("VT  0   1   2   3   4   5   6"));
  QVERIFY (    accept ("VT  0   1   2   3   4   5   6   7"));
  QVERIFY (    accept ("VT -0   1   2   3   4   5   6   7"));
  QVERIFY (    accept ("VT  0  -1   2   3   4   5   6   7"));
  QVERIFY (    accept ("VT  0   1  -2   3   4   5   6   7"));
  QVERIFY (    accept ("VT  0   1   2  -3   4   5   6   7"));
  QVERIFY (    accept ("VT  0   1   2   3  -4   5   6   7"));
  QVERIFY (    accept ("VT  0   1   2   3   4  -5   6   7"));
  QVERIFY (    accept ("VT  0   1   2   3   4   5  -6   7"));
  QVERIFY (    accept ("VT  0   1   2   3   4   5   6  -7"));
  QVERIFY (    accept ("VT  0.0 1   2   3   4   5   6   7"));
  QVERIFY (    accept ("VT  0   1.0 2   3   4   5   6   7"));
  QVERIFY (    accept ("VT  0   1   2.0 3   4   5   6   7"));
  QVERIFY (    accept ("VT  0   1   2   3.0 4   5   6   7"));
  QVERIFY (    accept ("VT  0   1   2   3   4.0 5   6   7"));
  QVERIFY (    accept ("VT  0   1   2   3   4   5.0 6   7"));
  QVERIFY (    accept ("VT  0   1   2   3   4   5   6.0 7"));
  QVERIFY (    accept ("VT  0   1   2   3   4   5   6   7.0"));
  QVERIFY (not accept ("VT  a   1   2   3   4   5   6   7"));
  QVERIFY (not accept ("VT  0   a   2   3   4   5   6   7"));
  QVERIFY (not accept ("VT  0   1   a   3   4   5   6   7"));
  QVERIFY (not accept ("VT  0   1   2   a   4   5   6   7"));
  QVERIFY (not accept ("VT  0   1   2   3   a   5   6   7"));
  QVERIFY (not accept ("VT  0   1   2   3   4   a   6   7"));
  QVERIFY (not accept ("VT  0   1   2   3   4   5   a   7"));
  QVERIFY (not accept ("VT  0   1   2   3   4   5   6   a"));
  QVERIFY (not accept ("VT  0   1   2   3   4   5   6   7   8"));
  
  
  RecordPointer   sp = accept ("VT  0   1   2   3   4   5   6   7");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  QVERIFY (mp->x.toInt() == 0);
  QVERIFY (mp->y.toInt() == 1);
  QVERIFY (mp->z.toInt() == 2);
  QVERIFY (mp->nx.toInt()== 3);
  QVERIFY (mp->ny.toInt()== 4);
  QVERIFY (mp->nz.toInt()== 5);
  QVERIFY (mp->s.toInt() == 6);
  QVERIFY (mp->t.toInt() == 7);
}


void
Obj8::Test::Mnemonics::VLINE ()
{
  typedef Data::LineVertex Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  
  QVERIFY (not accept ("VLINE"));
  QVERIFY (not accept ("VLINE  0"));
  QVERIFY (not accept ("VLINE  0   1"));
  QVERIFY (not accept ("VLINE  0   1   2"));
  QVERIFY (not accept ("VLINE  0   1   2   0.3"));
  QVERIFY (not accept ("VLINE  0   1   2   0.3  0.4"));
  QVERIFY (    accept ("VLINE  0   1   2   0.3  0.4  0.5"));
  QVERIFY (    accept ("VLINE  0.0 1   2   0.3  0.4  0.5"));
  QVERIFY (    accept ("VLINE  0   1.0 2   0.3  0.4  0.5"));
  QVERIFY (    accept ("VLINE  0   1   2.0 0.3  0.4  0.5"));
  QVERIFY (    accept ("VLINE -0   1   2   0.3  0.4  0.5"));
  QVERIFY (    accept ("VLINE  0  -1   2   0.3  0.4  0.5"));
  QVERIFY (    accept ("VLINE  0   1  -2   0.3  0.4  0.5"));
  QVERIFY (not accept ("VLINE  a   1   2   0.3  0.4  0.5"));
  QVERIFY (not accept ("VLINE  0   a   2   0.3  0.4  0.5"));
  QVERIFY (not accept ("VLINE  0   1   a   0.3  0.4  0.5"));
  QVERIFY (not accept ("VLINE  0   1   2     3  0.4  0.5"));
  QVERIFY (not accept ("VLINE  0   1   2   0.3    4  0.5"));
  QVERIFY (not accept ("VLINE  0   1   2   0.3  0.4    5"));
  QVERIFY (not accept ("VLINE  0   1   2  -0.3  0.4  0.5"));
  QVERIFY (not accept ("VLINE  0   1   2   0.3 -0.4  0.5"));
  QVERIFY (not accept ("VLINE  0   1   2   0.3  0.4 -0.5"));
  QVERIFY (not accept ("VLINE  0   1   2   0.3  0.4  0.5 0.6"));
  QVERIFY (not accept ("VLINE  0   1   2   0.3  0.4  0.5 6"));
  /*
   * The tuples of 9.7, 9.8 and 9.9 strings are *not* magic.
   */
  QVERIFY (not accept ("VLINE  0   1   2   9.7  9.7  9.7"));
  
  
  RecordPointer   sp = accept ("VLINE  0   1   2   0.3 0.4 0.5");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  QVERIFY (mp->x.toInt() == 0);
  QVERIFY (mp->y.toInt() == 1);
  QVERIFY (mp->z.toInt() == 2);
  QVERIFY (qFuzzyCompare(mp->r.toDouble(), 0.3));
  QVERIFY (qFuzzyCompare(mp->g.toDouble(), 0.4));
  QVERIFY (qFuzzyCompare(mp->b.toDouble(), 0.5));
}



void
Obj8::Test::Mnemonics::VLIGHT ()
{
  typedef Data::LightVertex Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  
  QVERIFY (not accept ("VLIGHT"));
  QVERIFY (not accept ("VLIGHT  0"));
  QVERIFY (not accept ("VLIGHT  0   1"));
  QVERIFY (not accept ("VLIGHT  0   1   2"));
  QVERIFY (not accept ("VLIGHT  0   1   2   0.3"));
  QVERIFY (not accept ("VLIGHT  0   1   2   0.3  0.4"));
  QVERIFY (    accept ("VLIGHT  0   1   2   0.3  0.4  0.5"));
  QVERIFY (    accept ("VLIGHT  0.0 1   2   0.3  0.4  0.5"));
  QVERIFY (    accept ("VLIGHT  0   1.0 2   0.3  0.4  0.5"));
  QVERIFY (    accept ("VLIGHT  0   1   2.0 0.3  0.4  0.5"));
  QVERIFY (    accept ("VLIGHT -0   1   2   0.3  0.4  0.5"));
  QVERIFY (    accept ("VLIGHT  0  -1   2   0.3  0.4  0.5"));
  QVERIFY (    accept ("VLIGHT  0   1  -2   0.3  0.4  0.5"));
  QVERIFY (not accept ("VLIGHT  a   1   2   0.3  0.4  0.5"));
  QVERIFY (not accept ("VLIGHT  0   a   2   0.3  0.4  0.5"));
  QVERIFY (not accept ("VLIGHT  0   1   a   0.3  0.4  0.5"));
  QVERIFY (not accept ("VLIGHT  0   1   2     3  0.4  0.5"));
  QVERIFY (not accept ("VLIGHT  0   1   2   0.3    4  0.5"));
  QVERIFY (not accept ("VLIGHT  0   1   2   0.3  0.4    5"));
  QVERIFY (not accept ("VLIGHT  0   1   2  -0.3  0.4  0.5"));
  QVERIFY (not accept ("VLIGHT  0   1   2   0.3 -0.4  0.5"));
  QVERIFY (not accept ("VLIGHT  0   1   2   0.3  0.4 -0.5"));
  QVERIFY (not accept ("VLIGHT  0   1   2   0.3  0.4  0.5 0.6"));
  QVERIFY (not accept ("VLIGHT  0   1   2   0.3  0.4  0.5 6"));
  /*
   * The tuples of 9.7, 9.8 and 9.9 strings are magic.
   */
  QVERIFY (not accept ("VLIGHT  0   1   2   9.6  9.6  9.6"));
  QVERIFY (    accept ("VLIGHT  0   1   2   9.7  9.7  9.7"));
  QVERIFY (    accept ("VLIGHT  0   1   2   9.8  9.8  9.8"));
  QVERIFY (    accept ("VLIGHT  0   1   2   9.9  9.9  9.9"));
  QVERIFY (not accept ("VLIGHT  0   1   2  10.0 10.0 10.0"));
  QVERIFY (not accept ("VLIGHT  0   1   2   9.7  9.8  9.7"));
  QVERIFY (not accept ("VLIGHT  0   1   2   9.7  9.7  9.8"));
  QVERIFY (not accept ("VLIGHT  0   1   2   9.70 9.7  9.7"));
  QVERIFY (not accept ("VLIGHT  0   1   2   9.7  9.70 9.7"));
  QVERIFY (not accept ("VLIGHT  0   1   2   9.7  9.7  9.70"));
  
  
  RecordPointer   sp = accept ("VLIGHT  0   1   2   9.7  9.7  9.7");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  QVERIFY (mp->x.toInt() == 0);
  QVERIFY (mp->y.toInt() == 1);
  QVERIFY (mp->z.toInt() == 2);
  QVERIFY (qFuzzyCompare(mp->r.toDouble(), 9.7));
  QVERIFY (qFuzzyCompare(mp->g.toDouble(), 9.7));
  QVERIFY (qFuzzyCompare(mp->b.toDouble(), 9.7));
}



void
Obj8::Test::Mnemonics::IDX ()
{
  typedef Data::Index Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  index<Data::Index, 1>(__func__);
  
  RecordPointer   sp = accept ("IDX 1");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY(mp);
  QVERIFY(mp->indices().size() == 1);
  QVERIFY(mp->index(0) == 1);
}



void
Obj8::Test::Mnemonics::IDX10 ()
{
  typedef Data::Index10 Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  index<Data::Index10, 10>(__func__);
  
  RecordPointer   sp = accept ("IDX10 1 2 3 4 5 6 7 8 9 10");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY(mp);
  QVERIFY(mp->indices().size() == 10);
  for (int i=0; i!=10; ++i) {
    QVERIFY(mp->index(i) == i+1);
  }
}



void
Obj8::Test::Mnemonics::TRIS ()
{
  typedef Command::Geometry::Triangles Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("TRIS"));
  QVERIFY (not accept ("TRIS   0"));
  QVERIFY (    accept ("TRIS   0   0"));
  QVERIFY (not accept ("TRIS 0.0   0"));
  QVERIFY (not accept ("TRIS   0 0.0"));
  QVERIFY (not accept ("TRIS   0   1"));
  QVERIFY (not accept ("TRIS   0   2"));
  QVERIFY (    accept ("TRIS   0   3"));
  QVERIFY (not accept ("TRIS   0   4"));
  QVERIFY (not accept ("TRIS   0   0   0"));
  
  RecordPointer   sp = accept ("TRIS 2 24");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  QVERIFY (mp->offset() == 2);
  QVERIFY (mp->count() == 24);
}



void
Obj8::Test::Mnemonics::LINES ()
{
  typedef Command::Geometry::Lines Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("LINES"));
  QVERIFY (not accept ("LINES   0"));
  QVERIFY (    accept ("LINES   0   0"));
  QVERIFY (not accept ("LINES 0.0   0"));
  QVERIFY (not accept ("LINES   0 0.0"));
  QVERIFY (not accept ("LINES   0   1"));
  QVERIFY (    accept ("LINES   0   2"));
  QVERIFY (not accept ("LINES   0   3"));
  QVERIFY (    accept ("LINES   0   4"));
  QVERIFY (not accept ("LINES   0   0   0"));
  
  RecordPointer   sp = accept ("LINES 2 24");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  QVERIFY (mp->offset() == 2);
  QVERIFY (mp->count() == 24);
}


void
Obj8::Test::Mnemonics::LIGHTS ()
{
  typedef Command::Geometry::Lights Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("LIGHTS"));
  QVERIFY (not accept ("LIGHTS   0"));
  QVERIFY (    accept ("LIGHTS   0   0"));
  QVERIFY (not accept ("LIGHTS 0.0   0"));
  QVERIFY (not accept ("LIGHTS   0 0.0"));
  QVERIFY (    accept ("LIGHTS   0   1"));
  QVERIFY (    accept ("LIGHTS   0   2"));
  QVERIFY (    accept ("LIGHTS   0   3"));
  QVERIFY (    accept ("LIGHTS   0   4"));
  QVERIFY (not accept ("LIGHTS   0   0   0"));
  
  RecordPointer   sp = accept ("LIGHTS 2 24");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  QVERIFY (mp->offset() == 2);
  QVERIFY (mp->count() == 24);
}


void
Obj8::Test::Mnemonics::LIGHT_NAMED ()
{
  typedef Command::Geometry::LightNamed Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  /*
   * XXX
   *   test acceptance.
   */
  
  RecordPointer   sp = accept ("LIGHT_NAMED a 1 2 3");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  QVERIFY (mp->m_name.value() == "a");
  QVERIFY (qFuzzyCompare(mp->m_x.toDouble(), 1));
  QVERIFY (qFuzzyCompare(mp->m_y.toDouble(), 2));
  QVERIFY (qFuzzyCompare(mp->m_z.toDouble(), 3));
}


void
Obj8::Test::Mnemonics::LIGHT_CUSTOM ()
{
  typedef Command::Geometry::LightCustom Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  /*
   * XXX
   *   test more acceptance.
   */
  QVERIFY (    accept ("LIGHT_CUSTOM   1 2 3   0.4 0.5 0.6 0.7   8   0.09 0.10  0.11 0.12   dataref"));
  QVERIFY (not accept ("LIGHT_CUSTOM   1 2 3   1.4 0.5 0.6 0.7   8   0.09 0.10  0.11 0.12   dataref"));
  QVERIFY (not accept ("LIGHT_CUSTOM   1 2 3   0.4 0.5 0.6 0.7   8   1.09 0.10  0.11 0.12   dataref"));
  QVERIFY (not accept ("LIGHT_CUSTOM   1 2 3   0.4 0.5 0.6 0.7   8   0.09 0.10  1.11 0.12   dataref"));
  
  RecordPointer   sp = accept ("LIGHT_CUSTOM   1 2 3   0.4 0.5 0.6 0.7   8   0.09 0.10  0.11 0.12   dataref");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  QVERIFY (qFuzzyCompare(mp->m_x.toDouble(), 1));
  QVERIFY (qFuzzyCompare(mp->m_y.toDouble(), 2));
  QVERIFY (qFuzzyCompare(mp->m_z.toDouble(), 3));
  QVERIFY (qFuzzyCompare(mp->m_r.toDouble(), 0.4));
  QVERIFY (qFuzzyCompare(mp->m_g.toDouble(), 0.5));
  QVERIFY (qFuzzyCompare(mp->m_b.toDouble(), 0.6));
  QVERIFY (qFuzzyCompare(mp->m_a.toDouble(), 0.7));
  QVERIFY (qFuzzyCompare(mp->m_s.toDouble(), 8));
  QVERIFY (qFuzzyCompare(mp->m_s1.toDouble(), 0.09));
  QVERIFY (qFuzzyCompare(mp->m_t1.toDouble(), 0.10));
  QVERIFY (qFuzzyCompare(mp->m_s2.toDouble(), 0.11));
  QVERIFY (qFuzzyCompare(mp->m_t2.toDouble(), 0.12));
  QVERIFY (mp->m_dataref.value() == "dataref");
}


void
Obj8::Test::Mnemonics::LIGHT_PARAM ()
{
  typedef Command::Geometry::LightParam Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  /*
   * XXX
   *   test more acceptance.
   */
  QVERIFY (    accept ("LIGHT_PARAM name  1 2 3"));
  QVERIFY (    accept ("LIGHT_PARAM name  1 2 3 hello world!"));
  
  RecordPointer   sp = accept ("LIGHT_PARAM name  1 2 3");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  QVERIFY (mp->m_name.value() == "name");
  QVERIFY (qFuzzyCompare(mp->m_x.toDouble(), 1));
  QVERIFY (qFuzzyCompare(mp->m_y.toDouble(), 2));
  QVERIFY (qFuzzyCompare(mp->m_z.toDouble(), 3));
  QVERIFY (mp->m_params.empty());
}


void
Obj8::Test::Mnemonics::LIGHT_SPILL_CUSTOM ()
{
  typedef Command::Geometry::LightSpillCustom Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  /*
   * XXX
   *   test more acceptance.
   */
  QVERIFY (not accept ("LIGHT_SPILL_CUSTOM   1 2 3   0.4 0.5 0.6 0.7   8   0.09 0.10 0.11   12   dataref"));
  QVERIFY (    accept ("LIGHT_SPILL_CUSTOM   1 2 3   0.4 0.5 0.6 0.7   8   1.00 0.00 0.00   12   dataref"));
  QVERIFY (    accept ("LIGHT_SPILL_CUSTOM   1 2 3   0.4 0.5 0.6 0.7   8   0.00 1.00 0.00   12   dataref"));
  QVERIFY (    accept ("LIGHT_SPILL_CUSTOM   1 2 3   0.4 0.5 0.6 0.7   8   0.00 0.00 1.00   12   dataref"));
  
  RecordPointer   sp = accept ("LIGHT_SPILL_CUSTOM   1 2 3   0.4 0.5 0.6 0.7   8   0.00 0.00 1.00   12   dataref");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  QVERIFY (qFuzzyCompare(mp->m_x.toDouble(), 1));
  QVERIFY (qFuzzyCompare(mp->m_y.toDouble(), 2));
  QVERIFY (qFuzzyCompare(mp->m_z.toDouble(), 3));
  QVERIFY (qFuzzyCompare(mp->m_r.toDouble(), 0.4));
  QVERIFY (qFuzzyCompare(mp->m_g.toDouble(), 0.5));
  QVERIFY (qFuzzyCompare(mp->m_b.toDouble(), 0.6));
  QVERIFY (qFuzzyCompare(mp->m_a.toDouble(), 0.7));
  QVERIFY (qFuzzyCompare(mp->m_s.toDouble(), 8));
  QVERIFY (qFuzzyCompare(mp->m_dx.toDouble(), 0));
  QVERIFY (qFuzzyCompare(mp->m_dy.toDouble(), 0));
  QVERIFY (qFuzzyCompare(mp->m_dz.toDouble(), 1));
  QVERIFY (qFuzzyCompare(mp->m_semi.toDouble(),12));
  QVERIFY (mp->m_dataref.value() == "dataref");
}




void
Obj8::Test::Mnemonics::smoke_black ()
{
  smoke<Command::Action::SmokeBlack> (__func__);
}



void
Obj8::Test::Mnemonics::smoke_white ()
{
  smoke<Command::Action::SmokeWhite> (__func__);
}



void
Obj8::Test::Mnemonics::ATTR_LOD ()
{
  typedef Command::State::Lod Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("ATTR_LOD"));
  QVERIFY (not accept ("ATTR_LOD 1"));
  QVERIFY (    accept ("ATTR_LOD 1 2"));
  QVERIFY (not accept ("ATTR_LOD 1.0 2"));
  QVERIFY (not accept ("ATTR_LOD 1 2.0"));
  QVERIFY (not accept ("ATTR_LOD 1.0 2.0"));
  QVERIFY (not accept ("ATTR_LOD a 2"));
  QVERIFY (not accept ("ATTR_LOD 1 a"));
  QVERIFY (not accept ("ATTR_LOD 1 2 3"));
  
  RecordPointer   sp = accept ("ATTR_LOD 1234 5678");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  
  QVERIFY (mp);
  QVERIFY (mp->nearDistance() == 1234);
  QVERIFY (mp->farDistance()  == 5678);
}



void
Obj8::Test::Mnemonics::ATTR_ambient_rgb ()
{
  material<Command::State::Ambient> (__func__);
}



void
Obj8::Test::Mnemonics::ATTR_specular_rgb ()
{
  material<Command::State::Specular> (__func__);
}



void
Obj8::Test::Mnemonics::ATTR_emission_rgb ()
{
  material<Command::State::Emission> (__func__);
}



void
Obj8::Test::Mnemonics::ATTR_shiny_rat ()
{
  typedef Command::State::ShinyRat Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("ATTR_shiny_rat"));
  QVERIFY (not accept ("ATTR_shiny_rat a"));
  QVERIFY (    accept ("ATTR_shiny_rat 0"));
  QVERIFY (    accept ("ATTR_shiny_rat 0.1"));
  QVERIFY (    accept ("ATTR_shiny_rat 0.75"));
  QVERIFY (    accept ("ATTR_shiny_rat 1.00"));
  QVERIFY (not accept ("ATTR_shiny_rat 1.01"));
  QVERIFY (not accept ("ATTR_shiny_rat 1 2"));
  
  RecordPointer   sp = accept ("ATTR_shiny_rat 0.23");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  
  QVERIFY (qFuzzyCompare(mp->ratio(), 0.23));
}



void
Obj8::Test::Mnemonics::ATTR_reset ()
{
  ignoredNoArguments (__func__);
}


void
Obj8::Test::Mnemonics::ATTR_poly_os ()
{
  typedef Command::State::PolygonOffset Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("ATTR_poly_os"));
  QVERIFY (    accept ("ATTR_poly_os 0"));
  QVERIFY (not accept ("ATTR_poly_os -1"));
  QVERIFY (not accept ("ATTR_poly_os 0.1"));
  QVERIFY (not accept ("ATTR_poly_os 0.75"));
  QVERIFY (not accept ("ATTR_poly_os 1.00"));  
  QVERIFY (    accept ("ATTR_poly_os 1"));
  QVERIFY (not accept ("ATTR_poly_os 1 2"));
  
  RecordPointer   sp = accept ("ATTR_poly_os 1234");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  QVERIFY (mp->offset() == 1234);
}



void
Obj8::Test::Mnemonics::ATTR_cockpit ()
{
  ignoredNoArguments (__func__);
}



void
Obj8::Test::Mnemonics::ATTR_no_cockpit ()
{
  ignoredNoArguments (__func__);
}



void
Obj8::Test::Mnemonics::ATTR_cockpit_region ()
{
  typedef Command::State::CockpitRegion Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("ATTR_cockpit_region"));
  QVERIFY (    accept ("ATTR_cockpit_region 0"));
  QVERIFY (not accept ("ATTR_cockpit_region a"));
  QVERIFY (not accept ("ATTR_cockpit_region 0 1"));
  
  RecordPointer   sp = accept ("ATTR_cockpit_region 1");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  /*
   * XXX
   *   data interfaces required.
   */
}



void
Obj8::Test::Mnemonics::ATTR_light_level ()
{
  typedef Command::State::LightLevel Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("ATTR_light_level"));
  QVERIFY (not accept ("ATTR_light_level 0"));
  QVERIFY (not accept ("ATTR_light_level 0 1"));
  QVERIFY (    accept ("ATTR_light_level 0 1 dataref"));
  QVERIFY (not accept ("ATTR_light_level a 1 dataref"));
  QVERIFY (not accept ("ATTR_light_level 0 a dataref"));
  QVERIFY (not accept ("ATTR_light_level 0 1 dataref 3"));
  
  RecordPointer   sp = accept ("ATTR_light_level 0 1 dataref");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  /*
   * XXX
   *   data interfaces required.
   */
}



void
Obj8::Test::Mnemonics::ATTR_light_level_reset ()
{
  ignoredNoArguments (__func__);
}



void
Obj8::Test::Mnemonics::ATTR_shadow_blend ()
{
  typedef Command::State::ShadowBlend Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("ATTR_shadow_blend"));
  QVERIFY (not accept ("ATTR_shadow_blend -1"));
  QVERIFY (    accept ("ATTR_shadow_blend  0"));
  QVERIFY (    accept ("ATTR_shadow_blend  0.9"));
  QVERIFY (    accept ("ATTR_shadow_blend  1"));
  QVERIFY (not accept ("ATTR_shadow_blend  2"));
  QVERIFY (not accept ("ATTR_shadow_blend  a"));
  QVERIFY (not accept ("ATTR_shadow_blend  0 1"));
  
  RecordPointer   sp = accept ("ATTR_shadow_blend  0.9");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  QVERIFY (qFuzzyCompare(mp->ratio(), 0.9));
}



void
Obj8::Test::Mnemonics::ATTR_draped ()
{
  typedef Command::State::Draped   Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (    accept ("ATTR_draped"));
  QVERIFY (not accept ("ATTR_draped 1"));
  
  RecordPointer   sp = accept ("ATTR_draped");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
}



void
Obj8::Test::Mnemonics::ATTR_no_draped ()
{
  typedef Command::State::NoDraped Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (    accept ("ATTR_no_draped"));
  QVERIFY (not accept ("ATTR_no_draped 1"));
  
  RecordPointer   sp = accept ("ATTR_no_draped");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
}



void
Obj8::Test::Mnemonics::ATTR_shadow ()
{
  typedef Command::State::Shadow Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (    accept ("ATTR_shadow"));
  QVERIFY (not accept ("ATTR_shadow 1"));
  
  RecordPointer   sp = accept ("ATTR_shadow");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
}



void
Obj8::Test::Mnemonics::ATTR_no_shadow ()
{
  typedef Command::State::NoShadow Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (    accept ("ATTR_no_shadow"));
  QVERIFY (not accept ("ATTR_no_shadow 1"));
  
  RecordPointer   sp = accept ("ATTR_no_shadow");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
}



void
Obj8::Test::Mnemonics::ATTR_hard ()
{
  hard<Command::State::Hard> (__func__);
}



void
Obj8::Test::Mnemonics::ATTR_no_hard ()
{
  ignoredNoArguments (__func__);
}



void
Obj8::Test::Mnemonics::ATTR_shade_flat ()
{
  ignoredNoArguments (__func__);
}



void
Obj8::Test::Mnemonics::ATTR_shade_smooth ()
{
  ignoredNoArguments (__func__);
}



void
Obj8::Test::Mnemonics::ATTR_no_depth ()
{
  ignoredNoArguments (__func__);
}



void
Obj8::Test::Mnemonics::ATTR_depth ()
{
  ignoredNoArguments (__func__);
}



void
Obj8::Test::Mnemonics::ATTR_no_cull ()
{
  ignoredNoArguments (__func__);
}



void
Obj8::Test::Mnemonics::ATTR_cull ()
{
  ignoredNoArguments (__func__);
}



void
Obj8::Test::Mnemonics::ATTR_no_blend ()
{
  typedef Command::State::NoBlend Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (    accept ("ATTR_no_blend"));
  QVERIFY (not accept ("ATTR_no_blend -1"));
  QVERIFY (    accept ("ATTR_no_blend  0"));
  QVERIFY (    accept ("ATTR_no_blend  0.9"));
  QVERIFY (    accept ("ATTR_no_blend  1"));
  QVERIFY (not accept ("ATTR_no_blend  2"));
  QVERIFY (not accept ("ATTR_no_blend  a"));
  QVERIFY (not accept ("ATTR_no_blend  0 1"));
  
  RecordPointer   sp = accept ("ATTR_no_blend  0.9");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  QVERIFY (qFuzzyCompare(mp->ratio(), 0.9));
}



void
Obj8::Test::Mnemonics::ATTR_blend ()
{
  ignoredNoArguments (__func__);
}



void
Obj8::Test::Mnemonics::ATTR_solid_camera ()
{
  ignoredNoArguments (__func__);
}



void
Obj8::Test::Mnemonics::ATTR_no_solid_camera ()
{
  ignoredNoArguments (__func__);
}



void
Obj8::Test::Mnemonics::ATTR_draw_enable ()
{
  ignoredNoArguments (__func__);
}



void
Obj8::Test::Mnemonics::ATTR_draw_disable ()
{
  ignoredNoArguments (__func__);
}



void
Obj8::Test::Mnemonics::ATTR_hard_deck ()
{
  hard<Command::State::Hard> (__func__);
}



void
Obj8::Test::Mnemonics::ANIM_begin ()
{
  ignoredNoArguments (__func__);
}



void
Obj8::Test::Mnemonics::ANIM_end ()
{
  ignoredNoArguments (__func__);
}



void
Obj8::Test::Mnemonics::ANIM_rotate ()
{
  typedef Command::Animation::Rotation Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("ANIM_rotate"));
  QVERIFY (not accept ("ANIM_rotate          1         2         3   0.4 0.5   0.6 0.7 dataref"));
  QVERIFY (    accept ("ANIM_rotate          1         0         0   0.4 0.5   0.6 0.7 dataref"));
  QVERIFY (    accept ("ANIM_rotate          0         1         0   0.4 0.5   0.6 0.7 dataref"));
  QVERIFY (    accept ("ANIM_rotate          0         0         1   0.4 0.5   0.6 0.7 dataref"));
  QVERIFY (    accept ("ANIM_rotate  0.5773502 0.5773502 0.5773502   0.4 0.5   0.6 0.7 dataref"));
  
  RecordPointer   sp = accept ("ANIM_rotate  0 0 -1   0.4 0.5   0.6 0.7 dataref");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  
  QVERIFY (qFuzzyCompare(mp->m_x.toDouble(),    0));
  QVERIFY (qFuzzyCompare(mp->m_y.toDouble(),    0));
  QVERIFY (qFuzzyCompare(mp->m_z.toDouble(),   -1));
  QVERIFY (qFuzzyCompare(mp->m_r1.toDouble(), 0.4));
  QVERIFY (qFuzzyCompare(mp->m_r2.toDouble(), 0.5));
  QVERIFY (qFuzzyCompare(mp->m_v1.toDouble(), 0.6));
  QVERIFY (qFuzzyCompare(mp->m_v2.toDouble(), 0.7));
  QVERIFY (mp->m_dataref.value() == "dataref");
}



void
Obj8::Test::Mnemonics::ANIM_trans ()
{
  typedef Command::Animation::Translation Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (    accept ("ANIM_trans  0    1    2      3    4    5      6    7    dataref"));
  QVERIFY (    accept ("ANIM_trans  0.0  1.0  2.0    3.0  4.0  5.0    6.0  7.0  dataref"));
  QVERIFY (    accept ("ANIM_trans -0   -1   -2     -3   -4   -5     -6   -7    dataref"));
  
  RecordPointer   sp = accept ("ANIM_trans -0   -1   -2     -3   -4   -5     -6   -7    dataref");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
}



void
Obj8::Test::Mnemonics::ANIM_hide ()
{
  typedef Command::Animation::Hide Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("ANIM_hide"));
  QVERIFY (not accept ("ANIM_hide 0"));
  QVERIFY (not accept ("ANIM_hide 0 1"));
  QVERIFY (    accept ("ANIM_hide 0 1 dataref"));
  
  RecordPointer   sp = accept ("ANIM_hide 0 1 dataref");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  /*
   * XXX
   *   Need member interfaces.
   */
}



void
Obj8::Test::Mnemonics::ANIM_show ()
{
  typedef Command::Animation::Show Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("ANIM_show"));
  QVERIFY (not accept ("ANIM_show 0"));
  QVERIFY (not accept ("ANIM_show 0 1"));
  QVERIFY (    accept ("ANIM_show 0 1 dataref"));
  
  RecordPointer   sp = accept ("ANIM_show 0 1 dataref");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  /*
   * XXX
   *   Need member interfaces.
   */
}



void
Obj8::Test::Mnemonics::ANIM_rotate_begin ()
{
  typedef Command::Animation::RotationBegin Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("ANIM_rotate_begin"));
  QVERIFY (not accept ("ANIM_rotate_begin 0"));
  QVERIFY (not accept ("ANIM_rotate_begin 0 1"));
  QVERIFY (not accept ("ANIM_rotate_begin 0 1 2"));
  QVERIFY (    accept ("ANIM_rotate_begin 0 1 2 dataref"));
  
  RecordPointer   sp = accept ("ANIM_rotate_begin 0 1 2 dataref");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  /*
   * XXX
   *   Need member interfaces.
   */
}



void
Obj8::Test::Mnemonics::ANIM_rotate_key ()
{
  typedef Command::Animation::RotationKey Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("ANIM_rotate_key"));
  QVERIFY (not accept ("ANIM_rotate_key 0"));
  QVERIFY (    accept ("ANIM_rotate_key 0 1"));
  QVERIFY (    accept ("ANIM_rotate_key 0.0 1"));
  QVERIFY (    accept ("ANIM_rotate_key 0 1.0"));
  QVERIFY (not accept ("ANIM_rotate_key a 1"));
  QVERIFY (not accept ("ANIM_rotate_key 0 a"));
  QVERIFY (not accept ("ANIM_rotate_key 0 1 2"));
  
  RecordPointer   sp = accept ("ANIM_rotate_key 0.0 1.0");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  /*
   * XXX
   *   Need member interfaces.
   */
}



void
Obj8::Test::Mnemonics::ANIM_rotate_end ()
{
  ignoredNoArguments (__func__);
}



void
Obj8::Test::Mnemonics::ANIM_trans_begin ()
{
  typedef Command::Animation::TranslationBegin Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("ANIM_trans_begin"));
  QVERIFY (    accept ("ANIM_trans_begin dataref"));
  
  RecordPointer   sp = accept ("ANIM_trans_begin dataref");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  /*
   * XXX
   *   Need member interfaces.
   */
}



void
Obj8::Test::Mnemonics::ANIM_trans_key ()
{
  typedef Command::Animation::TranslationKey Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("ANIM_trans_key"));
  QVERIFY (not accept ("ANIM_trans_key 0"));
  QVERIFY (not accept ("ANIM_trans_key 0  1"));
  QVERIFY (not accept ("ANIM_trans_key 0  1   2"));
  QVERIFY (    accept ("ANIM_trans_key 0  1   2   3"));
  QVERIFY (    accept ("ANIM_trans_key 0  1.0 2   3"));
  QVERIFY (    accept ("ANIM_trans_key 0  1   2.0 3"));
  QVERIFY (    accept ("ANIM_trans_key 0  1   2   3.0"));
  QVERIFY (not accept ("ANIM_trans_key 0  a   2   3"));
  QVERIFY (not accept ("ANIM_trans_key 0  1   a   3"));
  QVERIFY (not accept ("ANIM_trans_key 0  1   2   a"));
  
  RecordPointer   sp = accept ("ANIM_trans_key 0 1 2 3");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  /*
   * XXX
   *   Need member interfaces.
   */
}



void
Obj8::Test::Mnemonics::ANIM_trans_end ()
{
  ignoredNoArguments (__func__);
}



void
Obj8::Test::Mnemonics::ANIM_keyframe_loop ()
{
  typedef Command::Animation::KeyframeLoop Mnemonic;
  typedef QSharedPointer<Mnemonic> MnemonicPointer;
  
  QVERIFY (not accept ("ANIM_keyframe_loop"));
  QVERIFY (    accept ("ANIM_keyframe_loop 0"));
  QVERIFY (    accept ("ANIM_keyframe_loop -1.0"));
  QVERIFY (not accept ("ANIM_keyframe_loop a"));
  QVERIFY (not accept ("ANIM_keyframe_loop 0  1"));
  
  RecordPointer   sp = accept ("ANIM_keyframe_loop 0");
  MnemonicPointer mp = qSharedPointerDynamicCast<Mnemonic>(sp);
  QVERIFY (mp);
  /*
   * XXX
   *   Need member interfaces.
   */
}




QTEST_MAIN (Obj8::Test::Mnemonics);
