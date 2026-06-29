/*
 * Created by ckalo on 4/7/26.
 */

#include "fdebug_test.h"
#include "opengl.h"
#include "opengl_debug.h"
#include "shader_builder.h"

#define OPENGL_PROGRAM_TABLE_SIZE 10

struct opengl_program program_table[OPENGL_PROGRAM_TABLE_SIZE];
uint32 programs_count = 0;

static build_t
insert_program(GLint program)
{
    const build_t idx = programs_count;
    program_table[idx].program = program;
    program_table[idx].build_idx = idx;
    programs_count++;
    return idx;
}
struct opengl_program*
shader_program_get(build_t progidx)
{
    struct opengl_program* prog = &program_table[progidx];
    return prog;
}

void
opengl_program_init()
{
    for(uint32 i = 0; i < OPENGL_PROGRAM_TABLE_SIZE; i++) {
        program_table[i].program = INT_NULL;
        program_table[i].build_idx = INT_NULL;
    }
}

void
opengl_program_terminate()
{
    for(uint32 i = 0; i < programs_count; i++) {
        program_table[i].program = INT_NULL;
        program_table[i].build_idx = INT_NULL;
    }

    // TODO: free allocation (when implemented)
}

build_t
gfx_program_exists(const sididx_t sid_idx)
{
    build_t build_idx = shader_build_is_set(sid_idx);
    return build_idx;
}

struct shader_descriptor*
opengl_program_create(const sididx_t sid_idx, const build_t build_idx, const char* fs)
{
    struct shader_descriptor* desc = shader_builder_build(sid_idx, build_idx);
    GL_CHECK();
    const GLuint vertex_shader = gl_create_shader(OPENGL_VERTEX_SHADER);
    gl_shader_source(vertex_shader, 1, &(desc->vshader_code), NULL);
    gl_compile_shader(vertex_shader);
    opengl_debug_shader(vertex_shader);
    GL_CHECK();

    const GLuint fragment_shader = gl_create_shader(OPENGL_FRAGMENT_SHADER);
    gl_shader_source(fragment_shader, 1, &fs, NULL);
    gl_compile_shader(fragment_shader);
    opengl_debug_shader(fragment_shader);
    GL_CHECK();

    const GLuint program = gl_create_program();
    gl_attach_shader(program, vertex_shader);
    gl_attach_shader(program, fragment_shader);
    gl_link_program(program);
    opengl_debug_program(program);
    GL_CHECK();

    const build_t program_idx = insert_program((GLint) program);
    desc->build_idx = program_idx;

    // TODO: create uniform block

    gl_delete_shader(vertex_shader);
    gl_delete_shader(fragment_shader);
    GL_CHECK();

    engine_fdebug_flush_arena(ENGINE_FDEBUG_FILE_VSHADER);

    return desc;
}

void
opengl_program_destroy(const build_t build_idx)
{
    program_table[build_idx].program = INT_NULL;
    program_table[build_idx].build_idx = INT_NULL;
    /// reduce  count only if 'build_idx' is last elem in the table
    if(build_idx == programs_count) programs_count--;
}

void
opengl_program_find() {}

struct opengl_program*
opengl_program_get_program_byidx(const build_t build_idx) { return &program_table[build_idx]; }
