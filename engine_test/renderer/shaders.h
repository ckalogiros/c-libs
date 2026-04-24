/*
 * Created by ckalo on 4/2/26.
 */
#ifndef ENGINE_TEST_SHADERS_H
#define ENGINE_TEST_SHADERS_H

static const char* vertex_shader_text =
"#version 330\n"
"uniform mat4 MVP;\n"
"in vec3 vCol;\n"
"in vec2 vPos;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 330\n"
// "in vec4 out_color4;\n"
"in vec3 out_color3;\n"
"out vec4 fragment;\n"
"void main()\n"
"{\n"
// "    fragment = out_color4;\n"
"    fragment = vec4(out_color3, 1.0);\n"
"}\n";


#endif //ENGINE_TEST_SHADERS_H