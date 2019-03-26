#version 330

uniform sampler2D texture_unit[8];
uniform bool texture_unit_enabled[8];

uniform bool texture_draped_enabled = false;

uniform bool discard_translucent;
uniform bool texturing;

uniform bool light_enabled;
uniform int  light_count;
uniform vec3 light_ambient = vec3(0, 0, 0);
uniform vec3 light_attenuations[128];
uniform vec3 light_positions[128];
uniform vec3 light_colors[128];

uniform mat4 modelview;

in vec2 tex_coord;
in vec4 color;
in vec3 normal;
in vec4 vertex;

in vec3 world_normal;
in vec3 world_vertex;

out vec4 FragColor;

struct LitColor
{
  vec3 color;
  float illumation;
};

LitColor lighting()
{
  LitColor result = LitColor(light_ambient, 0);
  vec3 specular_accum = vec3(0,0,0);
  
  vec3 normal0 = normalize(world_normal);
  
  for (int i=0; i!=light_count; ++i) {
    vec3 lightdir = light_positions[i] - world_vertex;
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
    
    float attenuation = 1.0 / (light_attenuations[i].x + light_attenuations[i].y * distance + light_attenuations[i].z * distance * distance);
    
    result.color += attenuation * diffuse * light_colors[i];
    specular_accum += attenuation * specular * light_colors[i];
    result.illumation += diffuse;
  }
  
  if (texture_unit_enabled[1]) {
    vec4 specmap = texture(texture_unit[1], tex_coord);
    specular_accum *= specmap.a;
  }
  
  result.color = clamp(specular_accum + result.color, 0, 1);
  
  return result;
}

vec4 colorFromTexture(LitColor color)
{
  vec4 bright = vec4(color.color, 1);
  
  if (texture_unit_enabled[0]) {
    
    bright *= texture(texture_unit[0], tex_coord);
    
    if (texture_unit_enabled[2]) {
    
      vec4 dark = texture(texture_unit[2], tex_coord);
    
      //float brightness = dot(bright.rgb, vec3(0.299, 0.587, 0.114));
      
      bright = /* brightness * */ bright + (1-color.illumation) * dark;
    }
  }
  
  return bright;
}

void main(void)
{
  LitColor l;
  vec4 c;
  
  if (light_enabled) {
    l = lighting();
  } else {
    l.color = vec3(1,1,1);
    l.illumation = 1;
  }
  
  if (texturing)
    c = colorFromTexture(l);
  else
    c = color * vec4(l.color,1);
  
  FragColor = c;
}
