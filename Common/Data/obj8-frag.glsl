#version 330

uniform sampler2D texture_unit[8];
uniform bool texture_unit_enabled[8];

uniform bool texture_draped_enabled = false;

uniform bool discard_translucent;
uniform bool texturing;

uniform bool light_enabled;
uniform int  light_count;
uniform vec3 light_ambient = vec3(0, 0, 0);

struct Light {
  vec3  position;
  vec3  color;
  
  vec3  attenuation;
  float range;
  float rangeExp;
  
  vec3  spotDirection;
  float spotCutoffAngle;
  float spotExp;
};

uniform Light lights[64];

uniform mat4 modelview;

in vec2 tex_coord;
in vec4 color;

in vec3 world_normal;
in vec3 world_vertex;

out vec4 FragColor;

Light getLight(int i)
{
  Light l = lights[i];
  return (l);
}

vec3 lighting()
{
  vec3 normal0 = normalize(world_normal);
  
  vec3 lit = light_ambient;
  
  for (int i=0; i!=light_count; ++i) {
    Light light = getLight(i);
    vec3 lightdir = light.position - world_vertex;
    float distance = length(lightdir);
    
    vec3 lightdir0 = normalize(lightdir);
    float diffuse = dot(lightdir0, normal0);
    diffuse = clamp(diffuse, 0, 1);
    
    vec3 reflection = reflect(lightdir0, normal0);
    vec3 reflection0 = normalize(reflection);
    vec3 eye0 = normalize(world_vertex);
    
    float specular = dot(eye0, reflection0);
    specular = clamp(specular, 0, 1);
    specular = pow(specular, 128);
    
    float attenuation = 1.0;
    if (light.range <= 0) {
      attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * distance * distance);
    } else {
      attenuation = 1.0 - min(1.0, pow(distance/light.range, light.rangeExp));
    }
    
    if (0 < light.spotCutoffAngle && light.spotCutoffAngle < 180) {
      vec3 spotdir0 = normalize(light.spotDirection);
      float spotcutoff = cos(radians(light.spotCutoffAngle/2));
      float spotfactor = max(0, -dot(spotdir0, lightdir0));
      if (spotfactor < spotcutoff) {
        spotfactor = 0;
      } else {
        spotfactor = 1-pow(spotcutoff/spotfactor, light.spotExp);
      }
      attenuation *= spotfactor;
    }
    
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
