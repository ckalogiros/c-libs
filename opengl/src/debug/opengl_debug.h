/*
 * Created by ckalo on 8/17/26.
 * https://wikis.khronos.org/opengl/Debug_Output
 */
#ifndef GLAD_OPENGL_DEBUG_H
#define GLAD_OPENGL_DEBUG_H

#include "my_types.h"
#include "logger.h"


uint32 gl_check_error();

#define GFX_CHECK_ERROR() do { \
uint32 err = gl_check_error(); \
if (err != 0) { \
print("OpenGL Error 0x%x at %s:%d\n", err, __FILE__, __LINE__); \
} \
} while(0)

void gl_debug_shader(const uint32 shader);

void gl_debug_program(const uint32 program);

void gl_debug_attributes_in(const uint32 count);

void gl_debug_enable();

void gl_print_context_info();

#endif //GLAD_OPENGL_DEBUG_H