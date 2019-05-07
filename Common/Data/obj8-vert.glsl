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

struct Texturing {
  int regular;
  int normal;
  int lit;
  int draped;
};

layout (std140) uniform Std140Blob {
  Transform transform;
  Lighting  lighting;
  Texturing texturing;
  State     state;
  int end_indicator;
};

attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texcoord;
attribute vec4 a_color;

out vec2 tex_coord;
out vec4 color;

out vec3 world_vertex;
out vec3 world_normal;

void main(void)
{
  world_normal = vec3(transform.modelview * vec4(a_normal,0));
  world_vertex = vec3(transform.modelview * vec4(a_position,1));
  
  color = a_color;
  tex_coord = a_texcoord.xy;
  
  gl_Position = transform.projection * transform.modelview * vec4(a_position,1);
}
