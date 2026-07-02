#version 330 core

/// attributes: in
layout (location = 0) in vec4 generic_0;
layout (location = 1) in vec4 generic_1;
layout (location = 2) in vec4 generic_2;
layout (location = 3) in vec4 generic_3;

out vec4 out_color4;

uniform mat4 projection_matrix;

void main()
{
  float vpos2_0 = generic_0.x;
  float vpos2_1 = generic_0.y;
  float wpos3_0 = generic_0.z;
  float wpos3_1 = generic_0.w;
  float wpos3_2 = generic_1.x;
  float scale2_0 = generic_1.y;
  float scale2_1 = generic_1.z;
  float rot2_0 = generic_1.w;
  float rot2_1 = generic_2.x;
  float color4_0 = generic_2.y;
  float color4_1 = generic_2.z;
  float color4_2 = generic_2.w;
  float color4_3 = generic_3.x;


  /// calculate transform
  vec4 vpos = vec4(vpos2_0, vpos2_1, 1.0, 1.0);
  vpos = vec4(vpos.x*scale2_0, vpos.y*scale2_1, 0.*1., 0.*1.);
  vpos = mat4(
      rot2_0, -rot2_1, 0.0, 0.0, 
      rot2_1,  rot2_0, 0.0, 0.0, 
      0.0,     0.0,    1.0, 0.0, 
      0.0,     0.0,    0.0, 1.0) * vpos;
  vpos = vec4(vpos.x+wpos3_0, vpos.y+wpos3_1, 1.+wpos3_2, 1.+0.);
  gl_Position = projection_matrix * vpos;

  /// outputs assignment
  out_color4 = vec4(color4_0, color4_1, color4_2, color4_3);
  vec2 out_vpos = gl_Position.xy;
}
