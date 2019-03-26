#version 330

uniform sampler2D texture_unit[8];
uniform bool texture_unit_enabled[8];

uniform bool texture_draped_enabled = false;

uniform bool discard_translucent;
uniform bool texturing;

uniform bool light_enabled;
uniform int  light_count;
uniform vec3 light_ambient = vec3(0.1, 0.1, 0.1);
uniform vec3 light_attenuations[128];
uniform vec3 light_positions[128];
uniform vec3 light_colors[128];

uniform mat4 modelview;

in vec2 tex_coord;
in vec4 color;

in vec3 world_normal;
in vec3 world_vertex;

out vec4 FragColor;

vec3 lighting()
{
  vec3 normal0 = normalize(world_normal);
  
  vec3 lit = light_ambient;
  
  for (int i=0; i!=light_count; ++i) {
    vec3 lightdir = light_positions[i] - world_vertex;
    float distance = length(lightdir);
    
    vec3 lightdir0 = normalize(lightdir);
    float diffuse = dot(lightdir0, normal0);
    diffuse = clamp(diffuse, 0, 1);
    
    //lit += light_colors[i] * diffuse;
    
    vec3 reflection = reflect(lightdir0, normal0);
    vec3 reflection0 = normalize(reflection);
    vec3 eye0 = normalize(world_vertex);
    
    float specular = dot(eye0, reflection0);
    specular = clamp(specular, 0, 1);
    specular = pow(specular, 128);
    
    float attenuation = 1.0 / (light_attenuations[i].x + light_attenuations[i].y * distance + light_attenuations[i].z * distance * distance);
    
    lit += attenuation * (diffuse + specular) * light_colors[i];
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
  return bright;
}

void main(void)
{
  vec4 c;
  
  if (light_enabled) {
    
    vec3 light = lighting();
    
    if (texturing)
      c = colorFromTexture(light);
    else
      c = color * vec4(light,1);
  } else {
    c = color;
  }
  
  if (discard_translucent) {
    if (c.a != 1) {
      //discard;
      c = vec4(1,0,0,1);
    }
  }
  
  FragColor = c;
}
