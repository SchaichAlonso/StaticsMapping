#version 330

uniform sampler2D texture_unit[8];
uniform bool texture_unit_enabled[8];

uniform bool texture_draped_enabled = false;

uniform bool discard_translucent;
uniform bool texturing;

uniform bool light_enabled;
uniform int  light_count;

struct AttenuationData {
  vec3  attenuation;
  float range;
  float rangeExp;
};

struct SpotData {
  vec3  spotDirection;
  float spotCutoffAngle;
  float spotExp;
} spotData;

struct Light {
  vec3  position;
  vec3  color;
};

uniform mat4 modelview;

in vec2 tex_coord;
in vec4 color;

in vec3 world_normal;
in vec3 world_vertex;

out vec4 FragColor;

Light getLight(int i)
{
  Light l;
  
  l.position = texelFetch(texture_unit[7], ivec2(i,0), 0).xyz;
  l.color    = texelFetch(texture_unit[7], ivec2(i,1), 0).xyz;
  
  return l;
}

AttenuationData getAttenuation(int i)
{
  AttenuationData a;
  a.attenuation = texelFetch(texture_unit[7], ivec2(i,2), 0).xyz;
  vec2 blob     = texelFetch(texture_unit[7], ivec2(i,3), 0).xy;
  a.range = blob.x;
  a.rangeExp = blob.y;
  return a;
}
  
SpotData getSpotData(int i)
{
  SpotData s;
  s.spotDirection = texelFetch(texture_unit[7], ivec2(i,4), 0).xyz;
  vec2 blob       = texelFetch(texture_unit[7], ivec2(i,5), 0).xy;
  s.spotCutoffAngle = blob.x;
  s.spotExp = blob.y;
  return (s);
}

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
  AttenuationData attenuation = getAttenuation(i);
  if (attenuation.range <= 0) {
    return 1.0 / dot(attenuation.attenuation, vec3(1, distance, distance*distance));
  } else {
    return 1.0 - min(1.0, pow(distance/attenuation.range, attenuation.rangeExp));
  }
}

float spotFactor(int i, vec3 lightdir0)
{
  SpotData spot = getSpotData(i);
  
  if (180 <= spot.spotCutoffAngle) {
    return 1;
  }
  
  vec3 spotdir0 = normalize(spot.spotDirection);
  float spotcutoff = cos(radians(spot.spotCutoffAngle/2));
  float spotfactor = max(0, -dot(spotdir0, lightdir0));
  if (spotfactor < spotcutoff) {
    return 0;
  } else {
    return 1-pow(spotcutoff/spotfactor, spot.spotExp);
  }
}

vec3 lighting()
{
  vec3 normal0 = normalize(world_normal);
  
  vec3 lit = vec3(0, 0, 0);
  
  for (int i=0; i!=light_count; ++i) {
    Light light = getLight(i);
    vec3 lightdir = light.position - world_vertex;
    float distance = length(lightdir);
    
    vec3 lightdir0 = normalize(lightdir);
    float diffuse = dot(lightdir0, normal0);
    if (diffuse < 0) {
      continue;
    }
    
    float specular = specularReflection(lightdir0, normal0);
    
    float attenuation = attenuationFactor(i, distance);
    
    attenuation *= spotFactor(i, lightdir0);
    
    lit += attenuation * (diffuse + specular) * light.color;
  }
  
  lit = clamp(lit, 0, 1);
  
  return lit;
}

vec4 colorFromTexture(vec3 light)
{
  vec4 bright = vec4(light, 1);
  
  if (texture_unit_enabled[0]) {
    if (texture_draped_enabled) {
      bright *= texture(texture_unit[1], tex_coord);
    } else {
      bright *= texture(texture_unit[0], tex_coord);
    
      if (texture_unit_enabled[2]) {
      
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
  vec4 c;
  vec3 light;
  
  if (light_enabled) {
    light = lighting();
  } else {
    light = vec3(1,1,1);
  }

  if (texturing) {
    c = color * colorFromTexture(light);
  } else {
    c = color * vec4(light,1);
  }
  
  if (discard_translucent) {
    if (c.a != 1) {
      //discard;
      c = vec4(1,0,0,1);
    }
  }
  
  FragColor = c;
}
