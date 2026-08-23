/*
 * Created by ckalo on 8/17/26.
 * https://wikis.khronos.org/opengl/Debug_Output
 */

#include "opengl_debug.h"
#include <assert.h>
#include "glad.h"

#define OPENGL_PRINT_ALL_EXTENSIONS 0
#define OPENGL_PRINT_ALL_GLSL_VERSIONS 0

uint32 gl_check_error()
{
    GLenum err = glGetError();
    return err;
}

void gl_debug_shader(const uint32 shader)
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

void gl_debug_program(const uint32 program)
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

void gl_debug_attributes_in(const uint32 count)
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


/**/
// void APIENTRY opengl_debug_callback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
void APIENTRY opengl_debug_callback(uint32 source, uint32 type, uint32 id, uint32 severity, uint32 length, const char* message, const void* userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;


    //Error code 	Description
    int32 error_code_idx = -1;
    const char* error_code[] = {
        "GL_INVALID_ENUM: GLenum argument out of range.",
        "GL_INVALID_VALUE: Numeric argument out of range.",
        "GL_INVALID_OPERATION: Operation illegal in current state.",
        "GL_STACK_OVERFLOW: Function would cause a stack overflow.",
        "GL_STACK_UNDERFLOW: Function would cause a stack underflow.",
        "GL_OUT_OF_MEMORY: Not enough memory left to execute function.",
        "UNKNOWN",
    };
    if (id == GL_INVALID_ENUM)           error_code_idx =  0;
    else if (id == GL_INVALID_VALUE)     error_code_idx =  1;
    else if (id == GL_INVALID_OPERATION) error_code_idx =  2;
    else if (id == GL_STACK_OVERFLOW)    error_code_idx =  3;
    else if (id == GL_STACK_UNDERFLOW)   error_code_idx =  4;
    else if (id == GL_OUT_OF_MEMORY)     error_code_idx =  5;
    else                                 error_code_idx =  6;
    assert(error_code_idx > -1);


    int32 color_idx = -1;
    switch(severity) {
        case GL_DEBUG_SEVERITY_HIGH:         color_idx =  0; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       color_idx =  1; break;
        case GL_DEBUG_SEVERITY_LOW:          color_idx =  2; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: color_idx =  3; break;
    }
    assert(color_idx > -1);

    print("%s Error id:%d | %s\n msg:%s User param:%s \n",
        color_array[color_idx], id, error_code[error_code_idx], message, (char*) userParam);

    switch(source) {
        case GL_DEBUG_SOURCE_API:               print("  ERROR in: API, ");               break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     print("  ERROR in: Window System, ");     break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:   print("  ERROR in: Shader Compiler, ");   break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:       print("  ERROR in: Third Party, ");       break;
        case GL_DEBUG_SOURCE_APPLICATION:       print("  ERROR in: Application, ");       break;
        case GL_DEBUG_SOURCE_OTHER:             print("  ERROR in: Other, ");             break;
    }

    switch(type) {
        case GL_DEBUG_TYPE_ERROR:               print("  Type: Error\n");                 break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: print("  Type: Deprecated Behaviour\n");  break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  print("  Type: Undefined Behaviour\n");   break;
        case GL_DEBUG_TYPE_PORTABILITY:         print("  Type: Portability\n");           break;
        case GL_DEBUG_TYPE_PERFORMANCE:         print("  Type: Performance\n");           break;
        case GL_DEBUG_TYPE_MARKER:              print("  Type: Marker\n");                break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          print("  Type: Push Group\n");            break;
        case GL_DEBUG_TYPE_POP_GROUP:           print("  Type: Pop Group\n");             break;
        case GL_DEBUG_TYPE_OTHER:               print("  Type: Other\n");                 break;
    }

    const char* severity_info[] = {
        "OpenGL Error, shader compilation/linking or highly-dangerous undefined behavior ",
        "Major performance warning, shader compilation/linking warning, or the use of deprecated functionality ",
        "Redundant state change performance warning, or unimportant undefined behavior ",
        "Anything that isn't an error or performance issue. ",
    };
    switch(severity) {
        case GL_DEBUG_SEVERITY_HIGH:        print("  Severity: high\n  %s\n",        severity_info[0]); break;
        case GL_DEBUG_SEVERITY_MEDIUM:      print("  Severity: medium\n  %s\n",      severity_info[1]); break;
        case GL_DEBUG_SEVERITY_LOW:         print("  Severity: low\n  %s\n",         severity_info[2]); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:print("  Severity: notification\n %s\n", severity_info[3]); break;
    }
    print("\n%s", CLEAR);
}


void gl_debug_enable()
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback((void*)opengl_debug_callback, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
}


/* DEBUG-PRINT */
void gl_print_context_info()
{
    GLint major = 0, minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    print("%s Version Major:[%d] %s\n", br1, major, CLEAR);
    print("%s Version Minor:[%d] %s\n", br1, minor, CLEAR);

    const GLubyte* version = glGetString(GL_VERSION);
    print("%s Version:%s %s\n", br1, version, CLEAR);

    const GLubyte* vendor = glGetString(GL_VENDOR);
    print("%s Vendor:%s %s\n", br1, vendor, CLEAR);

    const GLubyte* renderer = glGetString(GL_RENDERER);
    print("%s Renderer:%s %s\n", br1, renderer, CLEAR);

    GLint numExtensions = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
    print("%s GL Num extensions: [%d] %s\n", br1, numExtensions, CLEAR);
#   if OPENGL_PRINT_ALL_EXTENSIONS
    for (GLuint i = 0; i < (GLuint)numExtensions; ++i) {
        const char* ext = (const char*)glGetStringi(GL_EXTENSIONS, i);
        print("%s [%d] %s %s\n", br1, i, ext, CLEAR);
    }
#   endif

    GLint context_profilei;
    const char* context_profile[3] = {
        "NULL",
        /// The available profiles are:
        "GL_CONTEXT_CORE_PROFILE_BIT",
        "GL_CONTEXT_COMPATIBILITY_PROFILE_BIT"
    };
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &context_profilei);
    print("%s Context Profile:%s %s\n", br1, context_profile[context_profilei], CLEAR);

    /* The available context flags are:
     * GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT
     *     The context is a forward compatible context.
     * GL_CONTEXT_FLAG_DEBUG_BIT
     *     The context is a debug context.
     * GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT
     *     The context supports robust memory access functionality.
     * GL_CONTEXT_FLAG_NO_ERROR_BIT
     *     The context does not report OpenGL errors.
     */
    GLint context_flagsi;
    const char* context_flags[5] = {
        "NULL",
        "GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT",
        "GL_CONTEXT_FLAG_DEBUG_BIT",
        "GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT",
        "GL_CONTEXT_FLAG_NO_ERROR_BIT"
    };
    glGetIntegerv(GL_CONTEXT_FLAGS, &context_flagsi);
    print("%s Context flags:%s %s\n", br1, context_flags[context_flagsi], CLEAR);

    /**********************************************************************************************
     * SHADING LANGUAGE
     */
    const GLubyte* glsl = glGetString(GL_SHADING_LANGUAGE_VERSION);
    print("%s GLSL:%s %s\n", br1, glsl, CLEAR);

    GLint num_glsl_versions = 0;
    glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &num_glsl_versions);
    print("%s GLSL num versions: [%d] %s\n", br1, num_glsl_versions, CLEAR);
#   if OPENGL_PRINT_ALL_GLSL_VERSIONS
    for (GLuint i = 0; i < (GLuint)num_glsl_versions; ++i) {
        const char* version = (const char*)glGetStringi(GL_SHADING_LANGUAGE_VERSION, i);
        print("%s      [%d] %s %s\n", br1, i, version, CLEAR);
    }
#   endif

}

/* DEBUG-FILE-PRINT */
/// TODO