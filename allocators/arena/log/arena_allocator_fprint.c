/*
 * Created by ckalo on 8/27/26.
 */

#include "arena_allocator_fprint.h"
#include "my_macros.h"
#include "file_sys_interface.h"
#include "arena_allocator_struct.h"


/***************************************************************************************************
 * FOR DEBUG */
/* output one debug file and register all created mcaches  */
static struct fs_t* fs_created_instances  = NULL;
static struct fs_t* fs_released_instances = NULL;
static struct fs_t* fs_all_instances_mem  = NULL;
/* FOR DEBUG
 **************************************************************************************************/


void fprint_arena_mem_data(struct arena_allocator_instance* arena) {

    if(fs_all_instances_mem == NULL) {
        fs_all_instances_mem = fs_create("Debug/allocators/arena/arena_all_mem.txt");
        fs_append(fs_all_instances_mem,
            "All allocated memory of arena allocator instance data\n"
            "________________________________________________________"
        );
    }

    fs_append(fs_all_instances_mem,
        "######################"
        "arena allocator:\n"
        "# instance_id         %d\n"
        "# sp_offset           %p\n"
        "# data.start          %p\n"
        "# data.size           %u\n"
        "# data.used_size      %u\n"
        "# data.mem_id         %d\n"
        "######################",
        arena->instance_id,
        arena->sp_offset,
        arena->data.start,
        arena->data.size,
        arena->data.used_size,
        arena->data.mem_id
        );
    fs_append_mem(fs_all_instances_mem, arena->data.start, arena->data.size, "fprint_arena_mem_data");
}