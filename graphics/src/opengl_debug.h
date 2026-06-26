/*
 * Created by ckalo on 4/16/26.
 */
#ifndef ENGINE_TEST_OPENGL_DEBUG_H
#define ENGINE_TEST_OPENGL_DEBUG_H

#include <stdlib.h>
#include <glad/gl.h>
#include "glfw3.h"
#include "logger.h"
#include "opengl.h"
#include "gfx_buffers.h"

/* TODO:
 * Basic Attribute Queries (using glGetVertexAttribiv):
    GLint params[4];

    // Enabled state
    glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &params[0]);

    // Buffer binding (which VBO)
    glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &params[0]);

    // Size (components per attribute: 1, 2, 3, 4)
    glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_SIZE, &params[0]);

    // Stride (bytes between vertices)
    glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &params[0]);

    // Data type (GL_FLOAT, GL_INT, etc.)
    glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_TYPE, &params[0]);

    // Normalized (GL_TRUE/GL_FALSE)
    glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &params[0]);

    // Divisor (for instancing)
    glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_DIVISOR, &params[0]);

    // Integer type (for integer attributes)
    glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_INTEGER, &params[0]);

    // Long type (for 64-bit)
    glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_LONG, &params[0]);

 * Pointer Query (using glGetVertexAttribPointerv):
    void* pointer;
    glGetVertexAttribPointerv(index, GL_VERTEX_ATTRIB_ARRAY_POINTER, &pointer);
    // Returns the offset (as void*) for the attribute

 * VAO and Buffer Queries:
    // VAO state
    GLint vao;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao);

    // Current array buffer
    GLint array_buffer;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &array_buffer);

    // Current element array buffer (for indices)
    GLint element_buffer;
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &element_buffer);

    // Current program
    GLint program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &program);

    // Buffer size (after binding)
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    GLint buffer_size;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &buffer_size);
    GLint buffer_usage;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_USAGE, &buffer_usage);

 * Shader Attribute Queries:
    // Get all active attributes
    GLint num_attributes;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &num_attributes);

    for(int i = 0; i < num_attributes; i++) {
        char name[256];
        GLsizei length;
        GLint size;
        GLenum type;
        glGetActiveAttrib(program, i, sizeof(name), &length, &size, &type, name);
        GLint location = glGetAttribLocation(program, name);

        printf("Attribute %d: '%s' location=%d size=%d type=0x%x\n",
               i, name, location, size, type);
    }

 * Quick Diagnostic Function:
    void quick_attrib_check(int expected_count)
    {
        printf("\n=== QUICK ATTRIB CHECK ===\n");
        for(int i = 0; i < expected_count; i++) {
            GLint enabled, buffer, stride;
            void* offset;

            glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
            glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &buffer);
            glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
            glGetVertexAttribPointerv(i, GL_VERTEX_ATTRIB_ARRAY_POINTER, &offset);

            printf("Attrib[%d]: en=%d buf=%d stride=%d off=%p\n",
                   i, enabled, buffer, stride, offset);

            if(i == 0 && enabled && buffer == 0)
                printf("  WARNING: Attribute 0 has no buffer bound!\n");
            if(i == 1 && enabled && buffer == 0)
                printf("  WARNING: Attribute 1 has no buffer bound!\n");
            if(i == 1 && enabled && offset == (void*)0)
                printf("  WARNING: Attribute 1 offset is 0 (may conflict with attrib 0)!\n");
        }
    }
 *
 */


// #define GL_CHECK()
#define GL_CHECK() do { \
GLenum err = glGetError(); \
if (err != GL_NO_ERROR) { \
print("OpenGL Error 0x%x at %s:%d\n", err, __FILE__, __LINE__); \
} \
} while(0)


static void
opengl_debug_shader(const GLuint shader)
{
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    print("%sshader:  %s%u%s compiled successfully %s\n", GREEN, YELLOW, shader, GREEN, CLEAR);
    if(!compiled) {
        char log[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        print("%sERROR shader: %u info:%s\n", RED, shader, log, CLEAR);
    }
}

static void
opengl_debug_program(const GLuint program)
{
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if(!linked) {
        char log[512];
        glGetProgramInfoLog(program, 512, NULL, log);
        print("%sERROR Program: %u link error: %s %s\n", RED, program, log, CLEAR);
    } else {
        print("%sProgram: %s%u%s linked successfully %s\n", GREEN, YELLOW, program, GREEN, CLEAR);
    }
}

static void
opengl_debug_attributes_in(const uint32 count)
{
    GLint bound_vbo;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &bound_vbo);
    print("bound vbo: %d\n", bound_vbo);

    for(uint32 i = 0; i < count; i++) {
        GLint enabled, stride, binding;
        void* offset;

        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &binding);
        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
        glGetVertexAttribPointerv(i, GL_VERTEX_ATTRIB_ARRAY_POINTER, &offset);

        print("Attribute[%u]: enabled=%d, buffer=%d, stride=%d, offset=%u\n",
              i, enabled, binding, stride, offset);
    }

}

static void APIENTRY opengl_debug_callback(GLenum source,
                                           GLenum type,
                                           unsigned int id,
                                           GLenum severity,
                                           GLsizei length,
                                           const char* message,
                                           const void* userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;


    //Error code 	Description
    GL_INVALID_ENUM; //GLenum argument out of range.
    GL_INVALID_VALUE; //Numeric argument out of range.
    GL_INVALID_OPERATION; //Operation illegal in current state.
    GL_STACK_OVERFLOW; //Function would cause a stack overflow.
    GL_STACK_UNDERFLOW; //Function would cause a stack underflow.
    GL_OUT_OF_MEMORY; //Not enough memory left to execute function.


    print("%sError id:%d | msg:%s User param:%s \n", RED, id, message, (char*) userParam);

    switch(source) {
        case GL_DEBUG_SOURCE_API: print("%s", "Source: API\n");
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: print("%s", "Source: Window System\n");
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: print("%s", "Source: Shader Compiler\n");
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY: print("%s", "Source: Third Party\n");
            break;
        case GL_DEBUG_SOURCE_APPLICATION: print("%s", "Source: Application\n");
            break;
        case GL_DEBUG_SOURCE_OTHER: print("%s", "Source: Other\n");
            break;
    }

    switch(type) {
        case GL_DEBUG_TYPE_ERROR: print("%s", "Type: Error\n");
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: print("%s", "Type: Deprecated Behaviour\n");
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: print("%s", "Type: Undefined Behaviour\n");
            break;
        case GL_DEBUG_TYPE_PORTABILITY: print("%s", "Type: Portability\n");
            break;
        case GL_DEBUG_TYPE_PERFORMANCE: print("%s", "Type: Performance\n");
            break;
        case GL_DEBUG_TYPE_MARKER: print("%s", "Type: Marker\n");
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP: print("%s", "Type: Push Group\n");
            break;
        case GL_DEBUG_TYPE_POP_GROUP: print("%s", "Type: Pop Group\n");
            break;
        case GL_DEBUG_TYPE_OTHER: print("%s", "Type: Other\n");
            break;
    }

    switch(severity) {
        case GL_DEBUG_SEVERITY_HIGH: print("%s", "Severity: high\n");
            break;
        case GL_DEBUG_SEVERITY_MEDIUM: print("%s", "Severity: medium\n");
            break;
        case GL_DEBUG_SEVERITY_LOW: print("%s", "Severity: low\n");
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: print("%s", "Severity: notification\n");
            break;
    }
    print("\n%s", CLEAR);
}


static void
opengl_debug_init()
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(opengl_debug_callback, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
}

static void debug_opengl(struct opengl_program* prog,
                  struct vertex_array_object* va,
                  struct vertex_buffer_object* vb,
                  const uint32 total_vertex_count,
                  const uint32 vshader_attribs_stride)

{
    print("\n%s--- OPENGL DEBUG ---%s\n", GREEN, CLEAR);

    // After shader_attributes_enable, verify the state:
    glBindVertexArray(va->vao); // Your VAO
    GL_CHECK();


    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    print("\nMax number of attributes: %d\n", nrAttributes);
    GL_CHECK();

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    print("\nViewport: x=%d y=%d w=%d h=%d\n", viewport[0], viewport[1], viewport[2], viewport[3]);
    GL_CHECK();

    print(""
          "program: %u, progidx:%u\n"
          "va: %u, vaoidx:%u\n"
          "vb: %u, vboidx:%u\n"
          "\n",
          prog->program, prog->build_idx,
          va->vao, va->idx,
          vb->vbo, vb->idx
    );

    // After gfx_vertex_buffer_update_data, read back the data
    glBindBuffer(GL_ARRAY_BUFFER, vb->vbo);
    uint32 stride = shader_attrib_get_stride(prog->build_idx);
    const uint32 vbdata_count = total_vertex_count * stride;
    GL_CHECK();

    // float* readback = (float*) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
    float* readback = (float*) glMapBuffer(GL_ARRAY_BUFFER, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
    // float* readback = (float*) malloc(vbdata_count * sizeof(float));
    // glGetBufferSubData(GL_ARRAY_BUFFER, 0, vbdata_count * sizeof(float), readback);
    GL_CHECK();




    uint32 vcnt = 1;
    if(TRUE && readback) {
        print("VBO contents:");
        const uint32 len = vbdata_count;
        for(int i = 0; i < len; i++) {
            float val = readback[i];
            if(i % vshader_attribs_stride == 0) {
                print("\n[%2u][%3u]: ", vcnt++, i);
            }

            if(val >= 0) print("%4.1f, ", i, val);
            else print("%4.1f, ", i, val);
        }
        print("\n\n");
        glUnmapBuffer(GL_ARRAY_BUFFER);
    GL_CHECK();

    } else {
        print("Failed to map buffer\n");
    }
}
#endif //ENGINE_TEST_OPENGL_DEBUG_H
