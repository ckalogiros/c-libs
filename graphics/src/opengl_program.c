/*
 * Created by ckalo on 4/7/26.
 */

#include "opengl.h"
#include "opengl_debug.h"
#include "shader_builder.h"

#define OPENGL_PROGRAM_TABLE_SIZE 10

struct opengl_program program_table[OPENGL_PROGRAM_TABLE_SIZE];
uint32 programs_count = 0;

static int32
insert_program(GLint program)
{
    const int32 idx = programs_count;
    program_table[idx].program = program;
    programs_count++;
    return idx;
}

void
opengl_program_init()
{
    for(uint32 i = 0; i < OPENGL_PROGRAM_TABLE_SIZE; i++) {
        program_table[i].program = INT_NULL;
        program_table[i].idx = INT_NULL;
    }
}

void
opengl_program_terminate()
{
    for(uint32 i = 0; i < programs_count; i++) {
        program_table[i].program = INT_NULL;
        program_table[i].idx = INT_NULL;
    }

    // TODO: free allocation (when implemented)
}

struct shader_descriptor
opengl_program_create(const struct gfx_sid sid, const char* fs)
{
    struct shader_descriptor desc = shader_builder_build(sid);
    const GLuint vertex_shader = gl_create_shader(OPENGL_VERTEX_SHADER);
    gl_shader_source(vertex_shader, 1, &(desc.vshader_code), NULL);
    gl_compile_shader(vertex_shader);
    opengl_debug_shader(vertex_shader);

    const GLuint fragment_shader = gl_create_shader(OPENGL_FRAGMENT_SHADER);
    gl_shader_source(fragment_shader, 1, &fs, NULL);
    gl_compile_shader(fragment_shader);
    opengl_debug_shader(fragment_shader);

    const GLuint program = gl_create_program();
    gl_attach_shader(program, vertex_shader);
    gl_attach_shader(program, fragment_shader);
    gl_link_program(program);
    opengl_debug_program(program);


    const progidx_t program_idx = insert_program((GLint) program);
    desc.progidx = program_idx;

    // TODO: create uniform block

    gl_delete_shader(vertex_shader);
    gl_delete_shader(fragment_shader);

    return desc;
}

void
opengl_program_destroy(const int32 program_idx)
{
    program_table[program_idx].program = INT_NULL;
    program_table[program_idx].idx = INT_NULL;
    /// reduce  count only if 'program_idx' is last elem in the table
    if(program_idx == programs_count) programs_count--;
}

void
opengl_program_find() {}

struct opengl_program
opengl_program_get_program_byidx(progidx_t idx) { return program_table[idx]; }
