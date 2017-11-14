#pragma once

#include <QtTest>

#include <Obj8/Record.hpp>
#include <Obj8/RecordFactory.hpp>

namespace Obj8
{
  namespace Test
  {
    struct Mnemonics : QObject
    {
      Q_OBJECT
    
    protected:
      RecordPointer accept (QString) const;
      
      void ignoredNoArguments (QString) const;
      
      template<typename T, int> void index (QString) const;
      template<typename T> void hard (QString) const;
      template<typename T> void layerGroup (QString, bool) const;
      template<typename T> void material (QString) const;
      template<typename T> void smoke (QString) const;
      template<typename T> void texture (QString, bool) const;
    
    private slots:
      
      void sequence ();
      void conditional ();
      
      void TEXTURE ();
      void TEXTURE_LIT ();
      void TEXTURE_NORMAL ();
      void __TEXTURE_DRAPED ();
      void POINT_COUNTS ();
      void slung_load_weight ();
      void ATTR_layer_group ();
      void COCKPIT_REGION ();
      void GLOBAL_no_blend ();
      void GLOBAL_shadow_blend ();
      void GLOBAL_specular ();
      void GLOBAL_tint ();
      void GLOBAL_no_shadow ();
      void SLOPE_LIMIT ();
      void TILTED ();
      void REQUIRE_WET ();
      void REQUIRE_DRY ();
      void ATTR_layer_group_draped ();
      void ATTR_LOD_draped ();
      void GLOBAL_cockpit_lit ();
      
      void VT ();
      void VLINE ();
      void VLIGHT ();
      void IDX ();
      void IDX10 ();
      
      void TRIS ();
      void LINES ();
      void LIGHTS ();
      void LIGHT_NAMED ();
      void LIGHT_CUSTOM ();
      void LIGHT_PARAM ();
      void LIGHT_SPILL_CUSTOM ();
    
      void smoke_black ();
      void smoke_white ();
      
      void ATTR_LOD ();
      void ATTR_ambient_rgb ();
      void ATTR_specular_rgb ();
      void ATTR_emission_rgb ();
      void ATTR_shiny_rat ();
      void ATTR_reset ();
      void ATTR_poly_os ();
      void ATTR_cockpit ();
      void ATTR_no_cockpit ();
      void ATTR_cockpit_region ();
      void ATTR_light_level ();
      void ATTR_light_level_reset ();
      void ATTR_shadow_blend ();
      void ATTR_draped ();
      void ATTR_no_draped ();
      void ATTR_shadow ();
      void ATTR_no_shadow ();
      void ATTR_hard ();
      void ATTR_no_hard ();
      void ATTR_shade_flat ();
      void ATTR_shade_smooth ();
      void ATTR_no_depth ();
      void ATTR_depth ();
      void ATTR_no_cull ();
      void ATTR_cull ();
      void ATTR_no_blend ();
      void ATTR_blend ();
      void ATTR_solid_camera ();
      void ATTR_no_solid_camera ();
      void ATTR_draw_enable ();
      void ATTR_draw_disable ();
      void ATTR_hard_deck ();
      
      void ANIM_begin ();
      void ANIM_end ();
      void ANIM_rotate ();
      void ANIM_trans ();
      void ANIM_hide ();
      void ANIM_show ();
      void ANIM_rotate_begin ();
      void ANIM_rotate_key ();
      void ANIM_rotate_end ();
      void ANIM_trans_begin ();
      void ANIM_trans_key ();
      void ANIM_trans_end ();
      void ANIM_keyframe_loop ();
    };
  }
}
