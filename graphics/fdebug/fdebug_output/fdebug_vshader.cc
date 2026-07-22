#version 330 core

/// attributes: in
layout (location = 0) in vec2 vpos2;
layout (location = 1) in vec3 wpos3;
layout (location = 2) in vec2 scale2;
layout (location = 3) in vec2 rot2;
layout (location = 4) in vec4 color4;

out vec4 out_color4;

uniform mat4 projMat;

void main()
{

  /// calculate transform
  vec4 vpos = vec4(vpos2.x, vpos2.y, 1.0, 1.0);
  vpos = vec4(vpos.x * scale2.x, vpos.y * scale2.y, 0. * 1., 0. * 1.);
  vpos = vec4(vpos.x + wpos3.x, vpos.y + wpos3.y, 1. + wpos3.z, 1. + 0.);
  gl_Position = projMat * vpos;

  /// outputs assignment
  out_color4 = vec4(color4.x, color4.y, color4.z, color4.w);
  vec2 out_vpos = gl_Position.xy;
}
