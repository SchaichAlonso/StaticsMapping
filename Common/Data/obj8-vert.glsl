#version 330

uniform mat4 modelview;
uniform mat4 projection;

attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texcoord;
attribute vec4 a_color;

out vec2 tex_coord;
out vec4 color;

out vec3 world_vertex;
out vec3 world_normal;

void lighting()
{
  world_normal = vec3(modelview * vec4(a_normal,0));
  world_vertex = vec3(modelview * vec4(a_position,1));
}

void main(void)
{
  lighting();
  color = a_color;
  tex_coord = a_texcoord.xy;
  gl_Position = projection * modelview * vec4(a_position,1);
}
