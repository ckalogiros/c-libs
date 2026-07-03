/*
 * Created by ckalo on 6/15/26.
 *
 * command buffer must:
 * 1. copy the params to a memory that the command buffer controls(allocators:{arena, cyclical arena}, )
 *
 * TODO:
 */

#include "command.h"

#include <assert.h>
#include <string.h>

#include "queue_allocator.h"
#include "stack_allocator.h"



memid_t mem_instance_id = INT_NULL;
void cmd_init_heap_mem()
{
    mem_instance_id = queue_allocator_create_new_instance(4);
}
/// store params to heap mem
void cmd_push_params(void* params, const uint32 size)
{
    const uint32 alignment = 4; /// in bytes
    void* address = queue_allocator_alloc(mem_instance_id, size + 1, alignment, 0x0);
    memcpy(address, params, size);
}
void* cmd_pop_params()
{
    void* address = queue_allocator_release(mem_instance_id);
    return address;
}

#define CMD_TABLE_COUNT 32
struct command cmd_buffer[CMD_TABLE_COUNT];
uint32 cmd_table_count = 0;


struct command* cmd_get_next_free()
{
    assert((cmd_table_count+1) < CMD_TABLE_COUNT);
    uint32 idx = cmd_table_count;
    cmd_table_count++;
    return &cmd_buffer[idx];
}
struct command* cmd_get_command(const uint32 cmd_idx)
{
    return &cmd_buffer[cmd_idx];
}


void cmd_add_command(void* fn, void* params, const uint32 params_size)
{
    if(mem_instance_id == INT_NULL) cmd_init_heap_mem();

    struct command* new_cmd = cmd_get_next_free();
    new_cmd->fn = fn;
    cmd_push_params(params, params_size);
    // new_cmd->params = params;
}

void cmd_run(void)
{
    for (uint32 i=0; i<cmd_table_count; i++) {
        struct command* cmd = cmd_get_command(i);
        // void* params = cmd->params;
        void* params = cmd_pop_params();
        cmd->fn(params);
    }

    /// RESET COMMAND BUFFER
    cmd_table_count = 0;
}