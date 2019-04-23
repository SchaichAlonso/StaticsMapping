#version 330

struct Spot {
  vec3  direction;
  float cutoffAngle;
  float exponent;
};

struct Attenuation {
  float range;
  float exponent;
} attenuation;

struct Light {
  vec3 color;
  vec3 position;
  
  Spot spot;
  Attenuation attenuation;
};

struct Transform
{
  mat4 projection;
  mat4 modelview;
};

struct Lighting
{
  int   count;
  vec3  ambient;
  Light light[128];
};

struct State
{
  bool texture_unit_enabled[8];
  bool texturing_enabled;
  bool lighting_enabled;
};

layout (std140) uniform Std140Blob {
  Transform transform;
  Lighting  lighting;
  State     state;
  int end_indicator;
};


uniform sampler2D texture_unit[8];

uniform bool texture_draped_enabled = false;

in vec2 tex_coord;
in vec4 color;

in vec3 world_normal;
in vec3 world_vertex;

out vec4 FragColor;

float specularReflection(vec3 lightdir0, vec3 normal0)
{
  vec3 reflection = reflect(lightdir0, normal0);
  vec3 reflection0 = normalize(reflection);
  vec3 eye0 = normalize(world_vertex);
    
  float specular = dot(eye0, reflection0);
  specular = clamp(specular, 0, 1);
  specular = pow(specular, 128);
  
  return (specular);
}

float attenuationFactor(int i, float distance)
{
  return 1.0 - pow(distance/lighting.light[i].attenuation.range, lighting.light[i].attenuation.exponent);
  /*
  AttenuationData attenuation = getAttenuation(i);
  if (attenuation.range <= 0) {
    return 1.0 / dot(attenuation.attenuation, vec3(1, distance, distance*distance));
  } else {
    return 1.0 - min(1.0, pow(distance/attenuation.range, attenuation.rangeExp));
  }
  */
}

float spotFactor(int i, vec3 lightdir0)
{
  if (180 <= lighting.light[i].spot.cutoffAngle) {
    return 1;
  }
  
  vec3 spotdir0 = normalize(lighting.light[i].spot.direction.xyz);
  float spotcutoff = cos(radians(lighting.light[i].spot.cutoffAngle/2));
  float spotfactor = max(0, -dot(spotdir0, lightdir0));
  if (spotfactor < spotcutoff) {
    return 0;
  } else {
    return 1-pow(spotcutoff/spotfactor, lighting.light[i].spot.exponent);
  }
}

vec3 colorFromLight(int i, vec3 normal)
{
  vec3 lightdir = lighting.light[i].position - world_vertex;
  float distance = length(lightdir);
  vec3 lightdir0 = normalize(lightdir);
    
  float diffuse = dot(lightdir0, normal);
  if (diffuse < 0) {
    return vec3(0,0,0);
  }
    
  float attenuation = attenuationFactor(i, distance);
  if (attenuation <= 0.0) {
    return vec3(0,0,0);
  }
    
  float specular = specularReflection(lightdir0, normal);
  
  attenuation *= spotFactor(i, lightdir0);
  
  return attenuation * (diffuse + specular) * lighting.light[i].color;
}

vec3 colorFromLighting()
{
  vec3 normal = normalize(world_normal);
  
  vec3 lit = lighting.ambient;
  
  for (int i=0; i!=lighting.count; ++i) {
    lit += colorFromLight(i, normal);
  }
  
  lit = clamp(lit, 0, 1);
  
  return lit;
}

vec4 colorFromTexture(vec3 light)
{
  vec4 bright = vec4(light, 1);
  
  if (state.texture_unit_enabled[0]) {
    if (texture_draped_enabled) {
      bright *= texture(texture_unit[1], tex_coord);
    } else {
      bright *= texture(texture_unit[0], tex_coord);
    
      if (state.texture_unit_enabled[2]) {
        vec4 dark = texture(texture_unit[2], tex_coord);
        float brightness = dot(bright.rgb, vec3(0.299, 0.587, 0.114));
        bright = brightness * bright + (1-brightness) * dark;
      }
    }
  }
  bright = clamp(bright, 0, 1);
  return bright;
}

void main(void)
{
  if (end_indicator != 0x55aa55aa) {
    FragColor = vec4(1,0,0,1);
    return;
  }

  vec4 c;
  vec3 light;
  
  if (state.lighting_enabled) {
    light = colorFromLighting();
  } else {
    light = vec3(1,1,1);
  }

  if (state.texturing_enabled) {
    c = color * colorFromTexture(light);
  } else {
    c = color * vec4(light,1);
  }
  
  FragColor = c;
}
