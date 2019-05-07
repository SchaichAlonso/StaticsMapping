#pragma once

#include <QtGui/QGenericMatrix>
#include <QtGui/QVector3D>
#include <QtGui/QVector4D>

#include "BufferObject.hpp"
#include "Light.hpp"


namespace OpenGL {
  struct Std140Blob {
    Std140Blob();
    void setModelview(const QMatrix4x4 &m);
    void setProjection(const QMatrix4x4 &m);
    
    void setAmbient(QColor ambient);
    void setLight(int n, LightPointer l);
    int  lights() const;
    int  activeLights() const;
    void setActiveLights(int active);
    
    void setTextureUnitEnabled(int unit, bool enabled);
    void setTexturingEnabled(bool enabled);
    void setLightingEnabled(bool enabled);
    
    void upload(BufferObjectPointer ubo);
    static BufferObjectPointer createBufferObject();
    static int size();
    
  protected:
    template<typename T> void write(T &target, const T &value);
    
    struct Transform {
      alignas(16) QGenericMatrix<4,4,float> projection;
      alignas(16) QGenericMatrix<4,4,float> modelview;
    };
    
    struct Lighting {
      alignas(04) int light_count;
      alignas(16) QVector3D ambient;
      struct Light {
        bool operator!=(const Light &other) const;
        
        alignas(16) QVector3D color;
        alignas(16) QVector3D position;
        
        struct Spot {
          alignas(16) QVector3D direction;
          alignas(04) float cutoffAngle;
          alignas(04) float exponent;
        };
        Spot spot;
        
        struct Attenuation {
          alignas(04) float range;
          alignas(04) float exponent;
        };
        Attenuation attenuation;
      } light_data[128];
    };
    
    struct Texturing {
      alignas(4) int regular;
      alignas(4) int normal;
      alignas(4) int lit;
      alignas(4) int draped;
    };
    
    struct State {
      struct { alignas(16) int value; } texture_unit_enabled[8];
      alignas(04) int texturing_enabled;
      alignas(04) int lighting_enabled;
    };
    
    alignas(16) Transform transform;
    alignas(16) Lighting  lighting;
    alignas(04) Texturing texturing;
    alignas(16) State     state;
    alignas(04) int end_indicator;
    bool dirty;
  };
}
